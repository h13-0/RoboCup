#ifndef _PID_H_
#define _PID_H_

//位置式PID
typedef struct {
	float setpoint;      //设定目标

	float proportion;   //比例常数
	float integral;      //积分常数
	float derivative;    //微分常数

	float _sum_error;    //误差累计

	float _error;        //最近一次误差
} PositionPID_t;

float PosPIDCalc(PositionPID_t *PID_t, float currentPoint);

//增量式PID
typedef struct {
	float setpoint;      //设定目标

	float proportion;   //比例常数
	float integral;      //积分常数
	float derivative;    //微分常数

	float _previousError;    //上上次误差

	float _lastError;    //上次误差
} IncrementalPID_t;

float IncPIDCalc(IncrementalPID_t *PID_t, float currentPoint);

#endif
