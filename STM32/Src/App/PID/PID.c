#include "PID.h"
#include <math.h>

float PosPIDCalc(PositionPID_t *PID_t, float currentPoint) {
	volatile float error = PID_t->setpoint - currentPoint;

	volatile float result;

	PID_t->_sum_error = PID_t->_sum_error + error;

	result =

			PID_t->proportion * (error) +                //P 比例运算

			PID_t->integral * PID_t->_sum_error +           //I 积分运算

			PID_t->derivative * (error - PID_t->_error);    //D 微分运算

	PID_t->_error = error;

	//简单校验输出
	if (isnan(result))
		return 0;
	else if (isinf(result) == 1)
		return 0;
	else if (isinf(result) == -1)
		return 0;

	return result;
}

float IncPIDCalc(IncrementalPID_t *PID_t, float currentPoint) {
	volatile float error = PID_t->setpoint - currentPoint;

	volatile float result;

	result =

			//比例运算, P * 一阶导数 -> p * (error - _lastError)
			(PID_t->proportion) * (error - PID_t->_lastError) +

			//积分运算, I * 原函数 -> I * error
			(PID_t->integral) * (error) +

			//微分运算, D * 二阶导数 -> D * [(error + _previousError) - 2 * _lastError]
			(PID_t->derivative) * ((error + PID_t->_previousError) - 2 * (PID_t->_lastError));

	//更新Error
	PID_t->_previousError = PID_t->_lastError;
	PID_t->_lastError = error;

	//简单校验输出
	if (isnan(result))
		return 0;
	else if (isinf(result) == 1)
		return 0;
	else if (isinf(result) == -1)
		return 0;

	return result;
}
