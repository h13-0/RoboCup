/*
 * PIDRealize.h
 *
 *  Created on: 2021Äê6ÔÂ8ÈÕ
 *      Author: h13
 */

#ifndef APP_PIDMANAGEMENT_PIDMANAGEMENT_H_
#define APP_PIDMANAGEMENT_PIDMANAGEMENT_H_

/**
 * @brief: PID Handler.
 * @note:
 * 		Responsible for PID calculation.
 * 		**Call frequency: 100Hz.**
 */
void PIDHandler(void);

/**
 * @group: Enable or disable part of PID calculations.
 */
void EnableDirctionPIDRealize(void);

void DisableDirctionPIDRealize(void);

void EnableForwardPIDRealize(void);

void DisableForwardPIDRealize(void);



#endif /* APP_PIDMANAGEMENT_PIDMANAGEMENT_H_ */
