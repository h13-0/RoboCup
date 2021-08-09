/*
 * ArmConrtolConfig.h
 *
 *  Created on: 2021Äê8ÔÂ7ÈÕ
 *      Author: h13
 */

#ifndef APP_ARMCONTROL_ARMCONTROLCONFIGS_H_
#define APP_ARMCONTROL_ARMCONTROLCONFIGS_H_

/**
 * @group: Arm size configs.
 */
#define ArmNode0_Height        (121)
#define ArmNode0_RotationRange (180)
#define ArmNode1_Length        (242)
#define ArmNode2_Length        (105)
#define ArmNode3_Length        (ArmNode1_Length - ArmNode2_Length)

/**
 * @group: Arm motion control priority configs.
 * @note:  The smaller the number, the higher the priority.
 */
#define AxialLengthControlPriority    1
#define Z_AxisHeightControlPriority   0

#if(AxialLengthControlPriority == Z_AxisHeightControlPriority)
#error "The priorities of 'AxialLengthControlPriority' and 'Z_AxisHeightControlPriority' must be different in 'ArmControlConfigs.h'!!!"
#endif

#endif /* APP_ARMCONTROL_ARMCONTROLCONFIGS_H_ */
