/*
 * ArmControl.c
 *
 *  Created on: 2021Äê8ÔÂ7ÈÕ
 *      Author: h13
 */

#include "ArmControl.h"
#include "RobotConfigs.h"

#include <math.h>
#include <stdint.h>

#include "Servo.h"
#include "PID.h"

#ifdef DEBUG
PositionPID_t X_AxisPID = { 0 };
PositionPID_t Y_AxisPID = { 0 };
#define xAxisPID X_AxisPID
#define yAxisPID Y_AxisPID
#else
static PositionPID_t xAxisPID = { 0 };
static PositionPID_t yAxisPID = { 0 };
#endif

struct ArmControlStatusStructure
{
	uint8_t X_AxisPID_Status : 1;
	uint8_t Y_AxisPID_Status : 1;
#if(ArmControlMethod == ClosedLoopGeometricControl)
	uint8_t Z_AxisPID_Status : 1;
#endif
};

static struct ArmControlStatusStructure status = { 0 };

#define enableX_AxisPID()       status.X_AxisPID_Status = 1
#define disableX_AxisPID()      status.X_AxisPID_Status = 0
#define getX_AxisPID_Status()   status.X_AxisPID_Status

#define enableY_AYisPID()       status.Y_AYisPID_Status = 1
#define disableY_AYisPID()      status.Y_AYisPID_Status = 0
#define getY_AYisPID_Status()   status.Y_AYisPID_Status

#if(ArmControlMethod == ClosedLoopGeometricControl)
#define enableZ_AZisPID()       status.Z_AZisPID_Status = 1
#define disableZ_AZisPID()      status.Z_AZisPID_Status = 0
#define getZ_AZisPID_Status()   status.Z_AZisPID_Status
#endif

static float absoluteZ_AxisHeight = 0;
static float relativeZ_AxisHeight = 0;

/**
 * @brief: Init arm control of the robot.
 */
void ArmControlInit(void)
{
	xAxisPID.proportion = 0.0;
	xAxisPID.integration = 0.0;
	xAxisPID.differention = 0.0;   //note: Try not to use differention.

	xAxisPID.configs.autoResetIntegration = disable;
	xAxisPID.configs.limitIntegration = enable;

	xAxisPID.maxAbsOutput = (Node0_ServoMaximumRotationAngle - Node0_ServoMinimumRotationAngle) / 75.0;
	xAxisPID.maximumAbsValueOfIntegrationOutput = xAxisPID.maxAbsOutput * 0.1;
}

typedef struct {
	double X, Y;
} Point_t;

typedef struct {
	Point_t Center;
	double Radius;
} Circle_t;

static uint8_t equals(float const VarA, float const VarB)
{
	return fabs(VarA - VarB) < 1e-9;
}

/**
 * @brief: Calculate the square of the distance between two points.
 */
static float calculateDistanceSquare(Point_t const* PointA, Point_t const* PointB)
{
	return (PointA -> X - PointB -> X) * (PointA -> X - PointB -> X) + (PointA -> Y - PointB -> Y) * (PointA -> Y - PointB -> Y);
}

/**
 * @brief: Calculate the distance between two points.
 */
static float calculateDistance(Point_t const* PointA, Point_t const* PointB)
{
	return sqrt(calculateDistanceSquare(PointA, PointB));
}

typedef enum
{
	TwoIntersectionPoints,
	OneIntersectionPoint,
	CirclesAreSame,
	NoIntersectionPoint,
} IntersectType_t;

/**
 * @brief: Calculate the point where the two circles intersect.
 *
 */
static IntersectType_t calculateIntersectionPoint(Circle_t Circles[], Point_t Points[])
{
	double d, a, b, c, p, q, r;
	double cos_value[2], sin_value[2];
	if (equals(Circles[0].Center.X, Circles[1].Center.X)
			&& equals(Circles[0].Center.Y, Circles[1].Center.Y)
			&& equals(Circles[0].Radius, Circles[1].Radius)) {
		return CirclesAreSame;
	}

	d = calculateDistance(&Circles[0].Center, &Circles[1].Center);
	if (d > Circles[0].Radius + Circles[1].Radius || d < fabs(Circles[0].Radius - Circles[1].Radius))
	{
		return NoIntersectionPoint;
	}

	a = 2.0 * Circles[0].Radius * (Circles[0].Center.X - Circles[1].Center.X);
	b = 2.0 * Circles[0].Radius * (Circles[0].Center.Y - Circles[1].Center.Y);
	c = Circles[1].Radius * Circles[1].Radius - Circles[0].Radius * Circles[0].Radius - calculateDistanceSquare(&Circles[0].Center, &Circles[1].Center);
	p = a * a + b * b;
	q = -2.0 * a * c;
	if (equals(d, Circles[0].Radius + Circles[1].Radius) || equals(d, fabs(Circles[0].Radius - Circles[1].Radius)))
	{
		cos_value[0] = -q / p / 2.0;
		sin_value[0] = sqrt(1 - cos_value[0] * cos_value[0]);

		Points[0].X = Circles[0].Radius * cos_value[0] + Circles[0].Center.X;
		Points[0].Y = Circles[0].Radius * sin_value[0] + Circles[0].Center.Y;


		if (!equals(calculateDistanceSquare(&Points[0], &Circles[1].Center), Circles[1].Radius * Circles[1].Radius))
		{
			Points[0].Y = Circles[0].Center.Y - Circles[0].Radius * sin_value[0];
		}

		return OneIntersectionPoint;
	}

	r = c * c - b * b;
	cos_value[0] = (sqrt(q * q - 4.0 * p * r) - q) / p / 2.0;
	cos_value[1] = (-sqrt(q * q - 4.0 * p * r) - q) / p / 2.0;
	sin_value[0] = sqrt(1 - cos_value[0] * cos_value[0]);
	sin_value[1] = sqrt(1 - cos_value[1] * cos_value[1]);

	Points[0].X = Circles[0].Radius * cos_value[0] + Circles[0].Center.X;
	Points[1].X = Circles[0].Radius * cos_value[1] + Circles[0].Center.X;
	Points[0].Y = Circles[0].Radius * sin_value[0] + Circles[0].Center.Y;
	Points[1].Y = Circles[0].Radius * sin_value[1] + Circles[0].Center.Y;


	if (!equals(calculateDistanceSquare(&Points[0], &Circles[1].Center), Circles[1].Radius * Circles[1].Radius))
	{
		Points[0].Y = Circles[0].Center.Y - Circles[0].Radius * sin_value[0];
	}
	if (!equals(calculateDistanceSquare(&Points[1], &Circles[1].Center), Circles[1].Radius * Circles[1].Radius))
	{
		Points[1].Y = Circles[0].Center.Y - Circles[0].Radius * sin_value[1];
	}
	if (equals(Points[0].Y, Points[1].Y) && equals(Points[0].X, Points[1].X))
	{
		if (Points[0].Y > 0)
		{
			Points[1].Y = -Points[1].Y;
		} else {
			Points[0].Y = -Points[0].Y;
		}
	}
	return TwoIntersectionPoints;
}

/**
 * @brief: Set claw position in **Polar coordinates**
 * @param:
 * 		RotationAngle: The Angle of rotation relative to the zero position. Range: [0, 180]
 * 		AxialLength:   The length of the axis relative to zero in millimeters.
 * 		Z_AxisHeight:  The height above the ground in millimeters.
 */
ArmControlResult_t SetOpenLoopClawPosition(float RotationAngle, float AxialLength, float Z_AxisHeight)
{
	//Check target value.
	if(Z_AxisHeight < ArmNode0_Height)
	{
		return TargetValueIsInvalid;
	}

	if(AxialLength - ArmNode3_Length < ArmNode2_Length - ArmNode1_Length)
	{
		return TargetValueIsInvalid;
	}

	if((RotationAngle < 0) || (RotationAngle > ArmNode0_RotationRange))
	{
		return TooFar;
	}

	//Calculate intersection points.
	Point_t node1Point = { 0 };
	Point_t node3Point = { 0 };
	node3Point.X = AxialLength - ArmNode3_Length;
	node3Point.Y = Z_AxisHeight - ArmNode0_Height;
	if((node3Point.X * node3Point.X + node3Point.Y * node3Point.Y) > ((ArmNode1_Length + ArmNode2_Length) * (ArmNode1_Length + ArmNode2_Length)))
	{
		return TooFar;
	}

	Circle_t nodeCircles[2];
	nodeCircles[0].Center = node1Point;
	nodeCircles[0].Radius = ArmNode1_Length;

	nodeCircles[1].Center = node3Point;
	nodeCircles[1].Radius = ArmNode2_Length;

	Point_t intersections[2] = { 0 };
	IntersectType_t intersectType;
	intersectType = calculateIntersectionPoint(nodeCircles, intersections);

	//Calculate angle.
	double node1[2] = { 0 };
	double node2[2] = { 0 };
	double node3 = 0.0;
	if((intersectType != OneIntersectionPoint) && (intersectType != TwoIntersectionPoints))
	{
		return TooFar;
	} else if(intersectType == TwoIntersectionPoints)
	{
		node1[0] = acos(( - intersections[0].X) / ArmNode1_Length) * 180.0 / 3.1415926;
		node1[1] = acos(( - intersections[1].X) / ArmNode1_Length) * 180.0 / 3.1415926;
		if(intersections[0].Y < node3Point.Y)
		{
			node2[0] = acos((node3Point.X - intersections[0].X) / ArmNode2_Length) * 180.0 / 3.1415926;
		} else {
			node2[0] = - acos((node3Point.X - intersections[0].X) / ArmNode2_Length) * 180.0 / 3.1415926;
		}

		if(intersections[1].Y < node3Point.Y)
		{
			node2[1] = acos((node3Point.X - intersections[1].X) / ArmNode2_Length) * 180.0 / 3.1415926;
		} else {
			node2[1] = - acos((node3Point.X - intersections[1].X) / ArmNode2_Length) * 180.0 / 3.1415926;
		}
	} else if (intersectType == OneIntersectionPoint)
	{
		node1[0] = acos(( - intersections[0].X) / ArmNode1_Length) * 180.0 / 3.1415926;
		if(intersections[0].Y < node3Point.Y)
		{
			node2[0] = - acos((node3Point.X - intersections[0].X) / ArmNode2_Length) * 180.0 / 3.1415926;
		} else {
			node2[0] = acos((node3Point.X - intersections[0].X) / ArmNode2_Length) * 180.0 / 3.1415926;
		}
	}

	//Select better intersection.
	uint8_t betterIntersectionID = -1;
	uint8_t isIntersection0_Avaliable = 1;
	uint8_t isIntersection1_Avaliable = 1;
	if(intersectType == OneIntersectionPoint)
	{
		isIntersection1_Avaliable = 0;
	}

	//Eliminate solutions that mechanical conflicts.
	if(node1[0] + node2[0] < 0)
	{
		isIntersection0_Avaliable = 0;
	}

	if((isIntersection1_Avaliable) && (node1[1] + node2[1] < 0))
	{
		isIntersection0_Avaliable = 0;
	}

	//Eliminate solutions beyond the range of motion.
	if((isIntersection0_Avaliable) && (node1[0] + node2[0] > 180))
	{
		isIntersection0_Avaliable = 0;
	}

	if((isIntersection1_Avaliable) && (node1[1] + node2[1] > 180))
	{
		isIntersection0_Avaliable = 0;
	}

	//Eliminate solutions that touch the table.
	if((isIntersection0_Avaliable) && (intersections[0].X > 0) && (intersections[0].Y < node3Point.Y))
	{
		isIntersection0_Avaliable = 0;
	}

	if((isIntersection1_Avaliable) && (intersections[1].X > 0) && (intersections[1].Y < node3Point.Y))
	{
		isIntersection1_Avaliable = 0;
	}

	//Choose the lower intersection.
	if((isIntersection0_Avaliable) && (isIntersection1_Avaliable))
	{
		if(intersections[0].Y > intersections[1].Y)
		{
			betterIntersectionID = 1;
		} else {
			betterIntersectionID = 0;
		}
	} else if(isIntersection0_Avaliable)
	{
		betterIntersectionID = 0;
	} else if(isIntersection1_Avaliable)
	{
		betterIntersectionID = 1;
	}

	if(betterIntersectionID >= 0)
	{
		node3 = node2[betterIntersectionID];
		node2[betterIntersectionID] += node1[betterIntersectionID];
		//Move the robot arm.
		ArmNode0_Rotate(RotationAngle);
		ArmNode1_Rotate(node1[betterIntersectionID]);
		ArmNode2_Rotate(node2[betterIntersectionID]);
		ArmNode3_Rotate(node3);
	}
	return ArmControlOK;
}

/**
 * @brief: Arm control PID calculate handler.
 * @note:  **The call frequency must be 50 Hz!!**
 */
void ArmControlPIDCalculateHandler(void)
{
	static float xAxisPID_Accumulator = 0;
	static float yAxisPID_Accumulator = 0;
#if(ArmControlMethod == ClosedLoopGeometricControl)
	static float zAxisPID_Accumulator = 0;
#endif

	Coordinates_t coordinates = { 0 };
	GetAppleCoordinates(&coordinates);
	if(GetCurrentTimeMillisecond() - coordinates.TimeStamp < (1000.0 * MaximumFPS_Fluctuation / AppleDetectionAverageFPS))
	{
		xAxisPID_Accumulator += PosPID_Calc(&X_AxisPID, coordinates.X);
		if(xAxisPID_Accumulator > Node0_ServoMaximumRotationAngle)
		{
			xAxisPID_Accumulator = Node0_ServoMaximumRotationAngle;
		} else if(xAxisPID_Accumulator < Node0_ServoMinimumRotationAngle) {
			xAxisPID_Accumulator = Node0_ServoMinimumRotationAngle;
		}

		yAxisPID_Accumulator += PosPID_Calc(&Y_AxisPID, coordinates.Y);
		if(yAxisPID_Accumulator > 600)
		{
			yAxisPID_Accumulator = 600;
		} else if(yAxisPID_Accumulator < 50) {
			yAxisPID_Accumulator = 50;
		}
		SetOpenLoopClawPosition(xAxisPID_Accumulator, yAxisPID_Accumulator, absoluteZ_AxisHeight);
	}

}

/**
 * @brief: Aim the mechanical claw at the apple.
 * @param:
 * 		float AbsoluteZ_AxisHeight:   Absolute Z-Axis height.
 * 		[float RelativeZ_AxisHeight]: Relative Z-Axis height.
 * 		mtime_t TimeOut:              Time out in millisecond.
 * @note:
 * 		When using open-loop control, the `RelativeZ_AxisHeight` does not take effect.
 */
void AimAtApple(float AbsoluteZ_AxisHeight, float RelativeZ_AxisHeight, mtime_t TimeOut)
{
	absoluteZ_AxisHeight = AbsoluteZ_AxisHeight;
	relativeZ_AxisHeight = RelativeZ_AxisHeight;
	enableX_AxisPID();

}

/**
 * @brief: Grab.
 * @TODO:  Use current to judge whether it is firmly grasped.
 */
void Grab(void)
{

}
