/*
 * DC_MOTOR.c
 *
 *  Created on: Oct 4, 2024
 *      Author: Mohamed
 */

#include "DC_MOTOR.h"
#include "gpio.h"
#include <avr/io.h>
#include "PWM.h"

/* Initialize the MOTOR */
void DC_Motor_init(void) {
	/* Set Direction of Input1 and Input2 pin as input */
	GPIO_setupPinDirection(DC_MOTOR_PORT_ID , IN1_PIN_ID , PIN_OUTPUT);
	GPIO_setupPinDirection(DC_MOTOR_PORT_ID , IN2_PIN_ID , PIN_OUTPUT);

	/* Stop the Motor IN1 = 0 & IN2 = 0 */
	GPIO_writePin(DC_MOTOR_PORT_ID , IN1_PIN_ID , LOGIC_LOW);
	GPIO_writePin(DC_MOTOR_PORT_ID , IN2_PIN_ID , LOGIC_LOW);
}




// Example usage for 50% duty cycle
void DC_Motor_Rotate(Dc_Motor_State state, uint8 duty_cycle_percentage) {


	if(duty_cycle_percentage > 100)
		duty_cycle_percentage = 100;

	PWM_Set_Duty_Cycle(duty_cycle_percentage);

    switch(state) {
        case DC_MOTOR_CW:
            GPIO_writePin(DC_MOTOR_PORT_ID, IN1_PIN_ID, LOGIC_HIGH);
            GPIO_writePin(DC_MOTOR_PORT_ID, IN2_PIN_ID, LOGIC_LOW);
            break;

        case DC_MOTOR_CCW:
            GPIO_writePin(DC_MOTOR_PORT_ID, IN1_PIN_ID, LOGIC_LOW);
            GPIO_writePin(DC_MOTOR_PORT_ID, IN2_PIN_ID, LOGIC_HIGH);
            break;

        case DC_MOTOR_STOP:
         default:
            GPIO_writePin(DC_MOTOR_PORT_ID, IN1_PIN_ID, LOGIC_LOW);
            GPIO_writePin(DC_MOTOR_PORT_ID, IN2_PIN_ID, LOGIC_LOW);
            break;

    }


}



/* Function to get the current status of the motor */
Dc_Motor_State DC_Motor_getStatus(void) {
	uint8 in1_state = GPIO_readPin(DC_MOTOR_PORT_ID, IN1_PIN_ID);
	uint8 in2_state = GPIO_readPin(DC_MOTOR_PORT_ID, IN2_PIN_ID);

	/* Determine the motor state based on the IN1 and IN2 pin states */
	if (in1_state == LOGIC_LOW && in2_state == LOGIC_LOW) {
		return DC_MOTOR_STOP;  // Motor is stopped
	} else if (in1_state == LOGIC_LOW && in2_state == LOGIC_HIGH) {
		return DC_MOTOR_CW;    // Motor is rotating clockwise
	} else if (in1_state == LOGIC_HIGH && in2_state == LOGIC_LOW) {
		return DC_MOTOR_CCW;   // Motor is rotating counterclockwise
	}

	// Default return to stop (safety in case of unexpected states)
	return DC_MOTOR_STOP;
}
