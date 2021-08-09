/*
 * ArmControl.c
 *
 *  Created on: 2021Äê8ÔÂ7ÈÕ
 *      Author: h13
 */

#include "ArmControl.h"
#include <math.h>
#include <stdint.h>

#include "ArmControlConfigs.h"

#include "Servo.h"

#include "JustFloat.h"

typedef struct{
	double X, Y;
} Point_t;

typedef struct{
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
ArmControlResult_t SetClawPosition(float RotationAngle, float AxialLength, float Z_AxisHeight)
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

	uint8_t betterIntersectionID = 0;

	if((intersectType != OneIntersectionPoint) && (intersectType != TwoIntersectionPoints))
	{
		return TooFar;
	} else if(intersectType == TwoIntersectionPoints)
	{
		if((intersections[0].Y < 0) && (intersections[1].Y > 0))
		{
			betterIntersectionID = 1;
		} else if((intersections[0].Y > 0) && (intersections[1].Y < 0))
		{
			betterIntersectionID = 0;
		} else if((intersections[0].Y > 0) && (intersections[1].Y > 0))
		{
			if((intersections[0].Y < node3Point.Y) && (intersections[1].Y < node3Point.Y))
			{
				return TooFar;
			}

			//
			if(intersections[0].X < intersections[1].X)
			{
				betterIntersectionID = 0;
			} else {
				betterIntersectionID = 1;
			}
		} else {
			return TooFar;
		}
	} else if (intersectType == OneIntersectionPoint)
	{
		betterIntersectionID = 0;
	}

	//Calculate angle.
	double node1 = acos(( - intersections[betterIntersectionID].X) / ArmNode1_Length) * 180.0 / 3.1415926;
	double node2 = acos((node3Point.X - intersections[betterIntersectionID].X) / ArmNode2_Length) * 180.0 / 3.1415926;
	double node3 = 0.0;

	//


	if(intersections[betterIntersectionID].Y >= node3Point.Y)
	{
		node3 = -node2;
		node2 = node1 - node2;
	} else {
		node3 = node2;
		node2 = node1 + node2;
	}

	if((node1 > 180) || (node2 > 180) || (node3 > 90))
	{
		return TooFar;
	}


	float data[] = {0, 0, intersections[betterIntersectionID].X, intersections[betterIntersectionID].Y, AxialLength, Z_AxisHeight};
	SendJustFloatFrame(data, 6);

	//Move the robot arm.
	ArmNode0_Rotate(RotationAngle);
	ArmNode1_Rotate(node1);
	ArmNode2_Rotate(node2);
	ArmNode3_Rotate(node3);

	return ArmControlOK;
}
