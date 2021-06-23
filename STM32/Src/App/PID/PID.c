/*
 * PID.c
 *
 *  Created on: 2021, 6, 21.
 *      Author: h13
 */

#include "PID.h"
#include <math.h>

/**
 * @brief:  Basic PIC Calculate.
 * @param:
 * 		BasicPID_t *PIDx:   PositionPID struct.
 * 		float currentPoint: Current system status.
 * @return:
 * 		Calculation result.
 */
float BasicPID_Calc(BasicPID_t *PIDx, float currentPoint)
{
	float error = PIDx->setpoint - currentPoint;
	float result;

	PIDx -> _sumError += error;

	result =
		//proportion * error +
		PIDx -> proportion * (error) +
		//integration * sumerror +
		PIDx -> integration * PIDx -> _sumError +
		//differention * error'
		PIDx -> differention * (error - PIDx -> _error);

	PIDx -> _error = error;

	//Check the output.
	if (!isnormal(result))
	{
		return 0;
	} else {
		if(result > (PIDx -> maxAbsOutput))
		{
			return PIDx -> maxAbsOutput;
		} else if(result < - PIDx -> maxAbsOutput)
		{
			return - PIDx -> maxAbsOutput;
		}
	}

	return result;
}

/**
 * @brief: Position PID calculate with expand functions.
 * @param:
 * 		PositionPID_t *PIDx: Position PID struct.
 * 		float currentPoint:  Current system status.
 * @return:
 * 		Calculation result.
 */
float PosPID_Calc(PositionPID_t *PIDx, float currentPoint)
{
	float error = PIDx->setpoint - currentPoint;
	float result;

	PIDx -> _sumError += error;

	//Expand functions.
	//Auto reset integration.
	if(PIDx -> configs.autoResetIntegration == enable)
	{
		if((error > 0) && (PIDx -> configs._lastResetIntergrationStatus == 0))
		{
			PIDx -> _sumError = 0;
			PIDx -> configs._lastResetIntergrationStatus = 1;
		}else if((error < 0) && (PIDx -> configs._lastResetIntergrationStatus != 0))
		{
			PIDx -> _sumError = 0;
			PIDx -> configs._lastResetIntergrationStatus = 0;
		}
	}

	//Calculate the integral output.
	result = PIDx -> integration * PIDx -> _sumError;

	//Expand functions.
	//Limit integration.
	if(PIDx -> configs.limitIntegration == enable)
	{
		if(isnormal(result))
		{
			if(result > PIDx -> maximumAbsValueOfIntegrationOutput)
			{
				result = PIDx -> maximumAbsValueOfIntegrationOutput;
				PIDx -> _sumError = PIDx -> maximumAbsValueOfIntegrationOutput / PIDx -> integration;
			} else if(result < - PIDx -> maximumAbsValueOfIntegrationOutput)
			{
				result = - PIDx -> maximumAbsValueOfIntegrationOutput;
				PIDx -> _sumError = - (PIDx -> maximumAbsValueOfIntegrationOutput / PIDx -> integration);
			}
		} else {
			result = 0;
			PIDx -> _sumError = 0;
		}
	}

	//Calculate Output
	result =
		//proportion * error +
		PIDx -> proportion * (error) +
		//integral output +
		result +
		//differention * error'
		PIDx -> differention * (error - PIDx -> _error);

	//Update error.
	PIDx -> _error = error;

	//Check the output.
	if (!isnormal(result))
	{
		return 0;
	} else {
		if(result > (PIDx -> maxAbsOutput))
		{
			return PIDx -> maxAbsOutput;
		} else if(result < - PIDx -> maxAbsOutput)
		{
			return - PIDx -> maxAbsOutput;
		}
	}

	return result;
}

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
float PosPID_CalcWithCustDiff(PositionPID_t *PIDx, float currentPoint, float currentDifferention)
{
	float error = PIDx->setpoint - currentPoint;
	float result;

	PIDx -> _sumError += error;

	//Expand functions.
	//Auto reset integration.
	if(PIDx -> configs.autoResetIntegration == enable)
	{
		if((error > 0) && (PIDx -> configs._lastResetIntergrationStatus == 0))
		{
			PIDx -> _sumError = 0;
			PIDx -> configs._lastResetIntergrationStatus = 1;
		}else if((error < 0) && (PIDx -> configs._lastResetIntergrationStatus != 0))
		{
			PIDx -> _sumError = 0;
			PIDx -> configs._lastResetIntergrationStatus = 0;
		}
	}

	//Calculate the integral output.
	result = PIDx -> integration * PIDx -> _sumError;

	//Expand functions.
	//Limit integration.
	if(PIDx -> configs.limitIntegration == enable)
	{
		if(isnormal(result))
		{
			if(result > PIDx -> maximumAbsValueOfIntegrationOutput)
			{
				result = PIDx -> maximumAbsValueOfIntegrationOutput;
				PIDx -> _sumError = PIDx -> maximumAbsValueOfIntegrationOutput / PIDx -> integration;
			} else if(result < - PIDx -> maximumAbsValueOfIntegrationOutput)
			{
				result = - PIDx -> maximumAbsValueOfIntegrationOutput;
				PIDx -> _sumError = - (PIDx -> maximumAbsValueOfIntegrationOutput / PIDx -> integration);
			}
		} else {
			result = 0;
			PIDx -> _sumError = 0;
		}
	}

	//Calculate Output
	result =
		//proportion * error +
		PIDx -> proportion * (error) +
		//integral output +
		result +
		//differention * error'
		PIDx -> differention * currentDifferention;

	//Update error.
	PIDx -> _error = error;

	//Check the output.
	if (!isnormal(result))
	{
		return 0;
	} else {
		if(result > (PIDx -> maxAbsOutput))
		{
			return PIDx -> maxAbsOutput;
		} else if(result < - PIDx -> maxAbsOutput)
		{
			return - PIDx -> maxAbsOutput;
		}
	}

	return result;
}

/**
 * @brief: Position PID calculate with expand functions.
 * @param:
 * 		PositionPID_t *PIDx: Position PID struct.
 * 		float currentPoint:  Current system status.
 * 		float currentError:  Customed error.
 * @return:
 * 		Calculation results.
 * @note:
 * 		This function is suitable for continuous control systems with sudden change points.
 * 		Such as in the yaw Angle control plus or minus 180 degree problem.
 */
float PosPID_CalcWithCustErr(PositionPID_t *PIDx, float currentError)
{
	float result;

	PIDx -> _sumError += currentError;

	//Expand functions.
	//Auto reset integration.
	if(PIDx -> configs.autoResetIntegration == enable)
	{
		if((currentError > 0) && (PIDx -> configs._lastResetIntergrationStatus == 0))
		{
			PIDx -> _sumError = 0;
			PIDx -> configs._lastResetIntergrationStatus = 1;
		}else if((currentError < 0) && (PIDx -> configs._lastResetIntergrationStatus != 0))
		{
			PIDx -> _sumError = 0;
			PIDx -> configs._lastResetIntergrationStatus = 0;
		}
	}

	//Calculate the integral output.
	result = PIDx -> integration * PIDx -> _sumError;

	//Expand functions.
	//Limit integration.
	if(PIDx -> configs.limitIntegration == enable)
	{
		if(isnormal(result))
		{
			if(result > PIDx -> maximumAbsValueOfIntegrationOutput)
			{
				result = PIDx -> maximumAbsValueOfIntegrationOutput;
				PIDx -> _sumError = PIDx -> maximumAbsValueOfIntegrationOutput / PIDx -> integration;
			} else if(result < - PIDx -> maximumAbsValueOfIntegrationOutput)
			{
				result = - PIDx -> maximumAbsValueOfIntegrationOutput;
				PIDx -> _sumError = - (PIDx -> maximumAbsValueOfIntegrationOutput / PIDx -> integration);
			}
		} else {
			result = 0;
			PIDx -> _sumError = 0;
		}
	}

	//Calculate Output
	result =
		//proportion * error +
		PIDx -> proportion * (currentError) +
		//integral output +
		result +
		//differention * error'
		PIDx -> differention * (currentError - PIDx -> _error);

	//Update error.
	PIDx -> _error = currentError;

	//Check the output.
	if (!isnormal(result))
	{
		return 0;
	} else {
		if(result > (PIDx -> maxAbsOutput))
		{
			return PIDx -> maxAbsOutput;
		} else if(result < - PIDx -> maxAbsOutput)
		{
			return - PIDx -> maxAbsOutput;
		}
	}

	return result;
}

/**
 * @brief: Position PID calculate with expand functions.
 * @param:
 * 		PositionPID_t *PIDx:       Position PID struct.
 * 		float currentPoint:        Current system status.
 * 		float currentError:        Customed error.
 * 		float currentDifferention: Customed differention status.
 * @return:
 * 		Calculation results.
 * @note:
 * 		This function is suitable for continuous control systems with sudden change points and have a sensor with differential characteristic.
 */
float PosPID_CalcWithCustErrAndDiff(PositionPID_t *PIDx, float currentError, float currentDifferention)
{
	float result;

	PIDx -> _sumError += currentError;

	//Expand functions.
	//Auto reset integration.
	if(PIDx -> configs.autoResetIntegration == enable)
	{
		if((currentError > 0) && (PIDx -> configs._lastResetIntergrationStatus == 0))
		{
			PIDx -> _sumError = 0;
			PIDx -> configs._lastResetIntergrationStatus = 1;
		}else if((currentError < 0) && (PIDx -> configs._lastResetIntergrationStatus != 0))
		{
			PIDx -> _sumError = 0;
			PIDx -> configs._lastResetIntergrationStatus = 0;
		}
	}

	//Calculate the integral output.
	result = PIDx -> integration * PIDx -> _sumError;

	//Expand functions.
	//Limit integration.
	if(PIDx -> configs.limitIntegration == enable)
	{
		if(isnormal(result))
		{
			if(result > PIDx -> maximumAbsValueOfIntegrationOutput)
			{
				result = PIDx -> maximumAbsValueOfIntegrationOutput;
				PIDx -> _sumError = PIDx -> maximumAbsValueOfIntegrationOutput / PIDx -> integration;
			} else if(result < - PIDx -> maximumAbsValueOfIntegrationOutput)
			{
				result = - PIDx -> maximumAbsValueOfIntegrationOutput;
				PIDx -> _sumError = - (PIDx -> maximumAbsValueOfIntegrationOutput / PIDx -> integration);
			}
		} else {
			result = 0;
			PIDx -> _sumError = 0;
		}
	}

	//Calculate Output
	result =
		//proportion * error +
		PIDx -> proportion * (currentError) +
		//integral output +
		result +
		//differention * error'
		PIDx -> differention * currentDifferention;

	//Update error.
	PIDx -> _error = currentError;

	//Check the output.
	if (!isnormal(result))
	{
		return 0;
	} else {
		if(result > (PIDx -> maxAbsOutput))
		{
			return PIDx -> maxAbsOutput;
		} else if(result < - PIDx -> maxAbsOutput)
		{
			return - PIDx -> maxAbsOutput;
		}
	}

	return result;
}

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
float IncPID_Calc(IncrementalPID_t *PIDx, float currentPoint)
{
	float error = PIDx->setpoint - currentPoint;
	float result;

	result =
		//proportion * errpr' -> proportion * (error - _lastError) +
		(PIDx -> proportion) * (error - PIDx -> _lastError) +
		//integration * error -> I * error +
		(PIDx -> integration) * (error) +
		//differention * error'' -> differention * [(error + _previousError) - 2 * _lastError]
		(PIDx -> differention) * ((error + PIDx -> _previousError) - 2 * (PIDx -> _lastError));

	//Update errors.
	PIDx -> _previousError = PIDx -> _lastError;
	PIDx -> _lastError = error;

	if(!isnormal(result))
	{
		return 0;
	} else {
		if(result > PIDx -> maxAbsOutput)
		{
			return PIDx -> maxAbsOutput;
		} else if(result < - PIDx -> maxAbsOutput)
		{
			return - PIDx -> maxAbsOutput;
		}
	}

	return result;
}
