/*
 * PIR.c
 *
 *  Created on: Oct 28, 2024
 *      Author: Mohamed
 */

#include "PIR.h"     // Include header file for PIR sensor configuration and function declarations
#include "gpio.h"    // Include header file for GPIO configurations and functions


/*
 * Function: PIR_init
 * -------------------
 * Initializes the PIR sensor by setting up the pin direction.
 * PIR_PORT_ID: The port where the PIR sensor is connected.
 * PIR_PIN_ID: The specific pin of the port used for PIR sensor input.
 */
void PIR_init(){
	GPIO_setupPinDirection(PIR_PORT_ID, PIR_PIN_ID, PIN_INPUT);  // Set the PIR pin as an input
}


/*
 * Function: PIR_Motion
 * ---------------------
 * Reads and returns the state of the PIR sensor.
 * Returns:
 *    uint8 motion - The current state of the PIR sensor (1 if motion is detected, 0 otherwise).
 */
uint8 PIR_Motion(){

	uint8 motion = GPIO_readPin(PIR_PORT_ID, PIR_PIN_ID);  // Read PIR sensor pin value

	return motion;  // Return motion status
}
