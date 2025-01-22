/*
 * timer.h
 *
 *  Created on: Oct 27, 2024
 *      Author: Mohamed
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "std_types.h"

/*******************************************************************************
 *                                Structs and Enums                            *
 *******************************************************************************/

/* Enum to select the Timer ID */
typedef enum{
	TIMER0,  /* Timer 0 */
	TIMER1,  /* Timer 1 */
	TIMER2   /* Timer 2 */
} Timer_ID_Type ;

/* Enum to select the clock prescaler for the timer */
typedef enum{
	NO_CLOCK,     /* Timer is off */
	F_CPU_CLOCK,  /* No prescaling, clock source is F_CPU */
	F_CPU_8,      /* Prescaler of 8 */
	F_CPU_64,     /* Prescaler of 64 */
	F_CPU_256,    /* Prescaler of 256 */
	F_CPU_1024    /* Prescaler of 1024 */
} Timer_ClockType;

/* Enum to choose the mode of operation for the timer */
typedef enum{
	OVERFLOW, /* Overflow mode */
	COMPARE   /* Compare match mode */
} Timer_ModeType;

/* Structure to configure the timer parameters */
typedef struct{
	uint16 timer_InitialValue;         /* Initial value for the timer counter */
	uint16 timer_compare_MatchValue;   /* Compare match value, used in compare mode only */
	Timer_ID_Type timer_ID;            /* Timer ID: TIMER0, TIMER1, or TIMER2 */
	Timer_ClockType timer_clock;       /* Clock prescaler type */
	Timer_ModeType timer_mode;         /* Timer mode: OVERFLOW or COMPARE */
} Timer_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Function to initialize the timer with specified configurations
 * Parameters:
 * - Config_Ptr: Pointer to a structure containing timer configurations
 */
void Timer_init(const Timer_ConfigType * Config_Ptr);

/*
 * Function to disable the specified timer
 * Parameters:
 * - timer_type: The timer to disable (TIMER0, TIMER1, TIMER2)
 */
void Timer_deInit(Timer_ID_Type timer_type);

/*
 * Function to set a callback function for a timer interrupt
 * Parameters:
 * - a_ptr: Pointer to the callback function
 * - a_timer_ID: The ID of the timer for which the callback is set
 */
void Timer_setCallBack(void(*a_ptr)(void), Timer_ID_Type a_timer_ID);

#endif /* TIMER_H_ */
