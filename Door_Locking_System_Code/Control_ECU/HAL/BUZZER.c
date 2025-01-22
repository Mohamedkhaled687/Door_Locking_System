/*
 * BUZZER.c
 *
 *  Created on: Oct 4, 2024
 *      Author: Mohamed
 */

#include "BUZZER.h"
#include "gpio.h"
#include <avr/io.h>


/* Initialize the BUZZER */

void Buzzer_init(void){

	// Set direction of BUZZER_PIN as input
	GPIO_setupPinDirection(BUZZER_PORT_ID , BUZZER_PIN_ID , PIN_OUTPUT);

	// Deactivates Buzzer at beginning

	GPIO_writePin(BUZZER_PORT_ID , BUZZER_PIN_ID , LOGIC_LOW);
}


/* Turn on Buzzer */

void Buzzer_on(void){
	GPIO_writePin(BUZZER_PORT_ID , BUZZER_PIN_ID , LOGIC_HIGH);
}

// Deactivate Buzzer
void Buzzer_off(void){
	GPIO_writePin(BUZZER_PORT_ID , BUZZER_PIN_ID , LOGIC_LOW);
}
