/*
 * PIR.h
 *
 *  Created on: Oct 28, 2024
 *      Author: Mohamed
 */

#ifndef PIR_H_
#define PIR_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/


#define MOTION 1
#define NO_MOTION 0
#define PIR_PORT_ID  PORTC_ID
#define PIR_PIN_ID 	 PIN2_ID


/*******************************************************************************
 *                                Functions Protoype                            *
 *******************************************************************************/

/*
 * Function: PIR_init
 * -------------------
 * Initializes the PIR sensor by setting up the pin direction.
 * PIR_PORT_ID: The port where the PIR sensor is connected.
 * PIR_PIN_ID: The specific pin of the port used for PIR sensor input.
 */

void PIR_init();

/*
 * Function: PIR_Motion
 * ---------------------
 * Reads and returns the state of the PIR sensor.
 * Returns:
 *    uint8 motion - The current state of the PIR sensor (1 if motion is detected, 0 otherwise).
 */
uint8 PIR_Motion();

#endif /* PIR_H_ */
