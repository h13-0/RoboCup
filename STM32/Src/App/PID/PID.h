/*
 * PID.h
 *
 *  Created on: 2021, 6, 21.
 *      Author: h13
 */

#ifndef _PID_H_
#define _PID_H_

#include <stdint.h>

/**
 * @group: Basic PID(Position PID only).
 * @note:  Try not to use basic PID because it doesn't work well in most cases.
 */

/**
 * @brief: Basic PID typedef struct.
 * @param:
 * 		float setpoint:     Target value of PID control system.
 * 		float proportion:   PID proportional coefficient.
 * 		float integration:  PID integral coefficient.
 * 		float differention: PID differential coefficient.
 * 		float maxAbsOutput: Maximum absolute value of output.
 */
typedef struct
{
	//Basic parameters of PID
	float setpoint;

	float proportion;
	float integration;
	float differention;

	float maxAbsOutput;

	//private realizations.
	float _sumError;
	float _error;
} BasicPID_t;

/**
 * @brief:  Basic PIC Calculate.
 * @param:
 * 		BasicPID_t *PIDx:   PositionPID struct.
 * 		float currentPoint: Current system status.
 * @return:
 * 		Calculation result.
 */
float BasicPID_Calc(BasicPID_t *PIDx, float currentPoint);

/**
 * @group: Position PID.
 */
typedef enum
{
	disable = 0,
	enable  = 1,
} FunctionStatus_t;

/**
 * @brief: Position PID typedef struct.
 * @param:
 * 		@Basic parameters are same as Basic PID.
 * 		@configs:
 * 			FunctionStatus_t limitIntegration:        Limit sum error.
 * 			FunctionStatus_t autoResetIntegration:    Automatically reset integration when the current value crosses the setpoint.
 * 			float maximumAbsValueOfIntegrationOutput: Integration term output limit.
 */
typedef struct
{
	//Basic parameters of PID
	float setpoint;

	float proportion;
	float integration;
	float differention;

	float maxAbsOutput;

	//private realizations.
	float _sumError;
	float _error;

	//Expand functions.
	struct
	{
		//integration term limiting.
		FunctionStatus_t limitIntegration : 1;

		//Automatically reset integration when crossing setpoint.
		FunctionStatus_t autoResetIntegration : 1;

		//private realizations.
		uint8_t _lastResetIntergrationStatus : 1;
		uint8_t _reserved : 5;
	} configs;

	//Expand functions realization.
	float maximumAbsValueOfIntegrationOutput;
} PositionPID_t;

/**
 * @brief: Position PID calculate with expand functions.
 * @param:
 * 		PositionPID_t *PIDx: Position PID struct.
 * 		float currentPoint:  Current system status.
 * @return:
 * 		Calculation result.
 */
float PosPID_Calc(PositionPID_t *PIDx, float currentPoint);

/**
 * @brief: Position PID calculate with expand functions.
 * @param:
 * 		PositionPID_t *PIDx:       Position PID struct.
 * 		float currentPoint:        Current system status.
 * 		float currentDifferention: Customed differention status.
 * @return:
 * 		Calculation results.
 * @note:
 * 		This function is suitable for the control system of sensor with differential characteristic.
 * 		Such as when you have an angular velocimeter and want to control the angle.
 */
float PosPID_CalcWithCustDiff(PositionPID_t *PIDx, float currentPoint, float currentDifferention);

/**
 * @brief: Position PID calculate with expand functions.
 * @param:
 * 		PositionPID_t *PIDx: Position PID struct.
 * 		float currentError:  Customed error.
 * @return:
 * 		Calculation results.
 * @note:
 * 		This function is suitable for continuous control systems with sudden change points.
 * 		Such as in the yaw Angle control plus or minus 180 degree problem.
 */
float PosPID_CalcWithCustErr(PositionPID_t *PIDx, float currentError);

/**
 * @brief: Position PID calculate with expand functions.
 * @param:
 * 		PositionPID_t *PIDx:       Position PID struct.
 * 		float currentError:        Customed error.
 * 		float currentDifferention: Customed differention status.
 * @return:
 * 		Calculation results.
 * @note:
 * 		This function is suitable for continuous control systems with sudden change points and have a sensor with differential characteristic.
 */
float PosPID_CalcWithCustErrAndDiff(PositionPID_t *PIDx, float currentError, float currentDifferention);

/**
 * @group: Incremental PID.
 */

/**
 * @brief: Incremental PID typedef struct.
 * @param:
 * 		@Basic parameters are same as Basic PID.
 */
typedef struct {
	//Basic parameters of PID
	float setpoint;

	float proportion;
	float integration;
	float differention;

	float maxAbsOutput;

	//private realizations.
	float _previousError;
	float _lastError;
} IncrementalPID_t;

/**
 * @brief: Incremental PID calculate.
 * @param:
 * 		IncrementalPID_t *PIDx: Incremental PID struct.
 * 		float currentPoint:      Current system status.
 * @return:
 * 		Calculation results.
 * @note:
 * 		You need to integrate and limit the output.
 */
float IncPID_Calc(IncrementalPID_t *PIDx, float currentPoint);

#endif /* _PID_H_ */
