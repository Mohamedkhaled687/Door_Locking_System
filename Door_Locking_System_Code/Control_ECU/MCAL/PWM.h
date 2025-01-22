/*
 * PWM.h
 *
 *  Created on: Oct 4, 2024
 *      Author: Mohamed
 */

#ifndef PWM_H_
#define PWM_H_

#include "std_types.h"

#define PWM_OUTPUT_PIN PIN3_ID

void PWM_Timer0_init(void);
void PWM_Set_Duty_Cycle(uint8 duty_cycle);
#endif /* PWM_H_ */
