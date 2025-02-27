/* timer.c
 *
 *  Created on: Oct 27, 2024
 *      Author: Mohamed
 */

#include "timer.h"
#include "gpio.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "common_macros.h"

/* Callback function pointers for each timer */
static volatile void (*g_callBackPtr_Timer0)(void) = NULL_PTR;
static volatile void (*g_callBackPtr_Timer1)(void) = NULL_PTR;
static volatile void (*g_callBackPtr_Timer2)(void) = NULL_PTR;

/* ISR for Timer0 Overflow */
ISR(TIMER0_OVF_vect)
{
    if (g_callBackPtr_Timer0 != NULL_PTR)
    {
        g_callBackPtr_Timer0(); /* Call the callback function for Timer0 Overflow */
    }
}

/* ISR for Timer0 Compare Match */
ISR(TIMER0_COMP_vect)
{
    if (g_callBackPtr_Timer0 != NULL_PTR)
    {
        g_callBackPtr_Timer0(); /* Call the callback function for Timer0 Compare Match */
    }
}

/*************************** TIMER1 *******************************/

/* ISR for Timer1 Overflow */
ISR(TIMER1_OVF_vect)
{
    if (g_callBackPtr_Timer1 != NULL_PTR)
    {
        g_callBackPtr_Timer1(); /* Call the callback function for Timer1 Overflow */
    }
}

/* ISR for Timer1 Compare Match */
ISR(TIMER1_COMPA_vect)
{
    if (g_callBackPtr_Timer1 != NULL_PTR)
    {
        g_callBackPtr_Timer1(); /* Call the callback function for Timer1 Compare Match */
    }
}

/************************** TIMER2 **************************/

/* ISR for Timer2 Overflow */
ISR(TIMER2_OVF_vect)
{
    if (g_callBackPtr_Timer2 != NULL_PTR)
    {
        g_callBackPtr_Timer2(); /* Call the callback function for Timer2 Overflow */
    }
}

/* ISR for Timer2 Compare Match */
ISR(TIMER2_COMP_vect)
{
    if (g_callBackPtr_Timer2 != NULL_PTR)
    {
        g_callBackPtr_Timer2(); /* Call the callback function for Timer2 Compare Match */
    }
}

/* Function to initialize the timer with configurations */
void Timer_init(const Timer_ConfigType *Config_Ptr)
{
    switch (Config_Ptr->timer_ID)
    {
        /* Initialize Timer0 */
        case TIMER0:



            TCNT0 = (uint8)(Config_Ptr->timer_InitialValue); /* Set initial count value */

            TCCR0 = ((TCNT0 & 0xF8) | (Config_Ptr->timer_clock)); /* Set clock source */

            TCCR0 |= (1 << FOC0); /* Force Output Compare for non-PWM mode */

            /* Configure Timer0 Mode */
            switch (Config_Ptr->timer_mode)
            {
                case OVERFLOW:
                    CLEAR_BIT(TCCR0, WGM00); /* Set to normal mode */
                    CLEAR_BIT(TCCR0, WGM01);

                    SET_BIT(TIMSK, TOIE0); /* Enable Timer0 Overflow interrupt */
                    break;

                case COMPARE:
                    SET_BIT(TCCR0, WGM01);   /* Set to CTC mode */
                    CLEAR_BIT(TCCR0, WGM00);

                    OCR0 = (uint8)(Config_Ptr->timer_compare_MatchValue); /* Set compare match value */

                    SET_BIT(TIMSK, OCIE0); /* Enable Timer0 Compare Match interrupt */
                    break;
            }
            break;

        /* Initialize Timer1 */
            case TIMER1:
                /* FOC1A = 1, FOC1B = 1 for non-PWM Modes */
                TCCR1A = (1 << FOC1A) | (1 << FOC1B);
                TCCR1B = 0;

                TCNT1 = Config_Ptr->timer_InitialValue; /* Set initial count value */
                TCCR1B |= (Config_Ptr->timer_clock); /* Start clock source */

                /* Configure Timer1 Mode */
                switch (Config_Ptr->timer_mode)
                {
                    case OVERFLOW:
                        CLEAR_BIT(TCCR1B, WGM13); /* Set to normal mode */
                        CLEAR_BIT(TCCR1B, WGM12);
                        CLEAR_BIT(TCCR1A, WGM11);
                        CLEAR_BIT(TCCR1A, WGM10);

                        SET_BIT(TIMSK, TOIE1); /* Enable Timer1 Overflow interrupt */
                        break;

                    case COMPARE:
                        SET_BIT(TCCR1B, WGM12); /* Set to CTC mode */
                        OCR1A = Config_Ptr->timer_compare_MatchValue; /* Set compare match value */
                        SET_BIT(TIMSK, OCIE1A); /* Enable Timer1 Compare Match interrupt */
                        break;
                }
                break;


        /* Initialize Timer2 */
        case TIMER2:
            TCCR2 = 0; /* Reset Timer2 control register */
            TCNT2 = 0; /* Reset Timer2 counter register */
            OCR2 = 0;  /* Reset Timer2 Output Compare register */

            CLEAR_BIT(TIMSK, OCIE2); /* Disable Timer2 Compare Match interrupt */
            CLEAR_BIT(TIMSK, TOIE2); /* Disable Timer2 Overflow interrupt */

            TCNT2 = (Config_Ptr->timer_InitialValue) & 0x00FF; /* Set initial count value */

            TCCR2 = (TCCR2 & 0xF8) | (Config_Ptr->timer_clock); /* Set clock source */

            /* Configure Timer2 Mode */
            switch (Config_Ptr->timer_mode)
            {
                case OVERFLOW:
                    CLEAR_BIT(TCCR2, WGM20); /* Set to normal mode */
                    CLEAR_BIT(TCCR2, WGM21);

                    SET_BIT(TCCR2, FOC2); /* Force Output Compare for non-PWM mode */

                    SET_BIT(TIMSK, TOIE2); /* Enable Timer2 Overflow interrupt */
                    break;

                case COMPARE:
                    CLEAR_BIT(TCCR2, WGM20); /* Set to CTC mode */
                    SET_BIT(TCCR2, WGM21);

                    SET_BIT(TCCR2, FOC2); /* Force Output Compare for non-PWM mode */

                    OCR2 = (Config_Ptr->timer_compare_MatchValue) & 0x00FF; /* Set compare match value */

                    SET_BIT(TIMSK, OCIE2); /* Enable Timer2 Compare Match interrupt */
                    break;
            }
            break;
    }
}

void Timer_deInit(Timer_ID_Type timer_type)
{
	switch(timer_type)
	{
	case TIMER0:

		/* Clear all registers of Timer0 */
		TCCR0 = 0;
		TCNT0 = 0;
		OCR0 = 0;

		/* Clear all interrupt enables for Timer0 (OCIE0=0, TOIE0=0) */
		CLEAR_BIT(TIMSK, OCIE0);
		CLEAR_BIT(TIMSK, TOIE0);
		break;

	case TIMER1:

		/* Clear all registers of Timer1 */
		TCCR1A = 0;
		TCCR1B = 0;
		TCNT1 = 0;
		OCR1A = 0;

		/* Clear all interrupt enables for Timer1 (TICIE1=0, OCIE1A=0, OCIE1B=0, TOIE1=0) */
		TIMSK &= 0xC3; // Clear bits OCIE1A, OCIE1B, and TOIE1
		break;

	case TIMER2:

		/* Clear all registers of Timer2 */
		TCCR2 = 0;
		TCNT2 = 0;
		OCR2 = 0;

		/* Clear all interrupt enables for Timer2 (OCIE2=0, TOIE2=0) */
		CLEAR_BIT(TIMSK, OCIE2);
		CLEAR_BIT(TIMSK, TOIE2);
		break;
	}
}

/* Function to set callback function for a specific timer interrupt */
void Timer_setCallBack(void (*a_ptr)(void), Timer_ID_Type a_timer_ID)
{
    switch (a_timer_ID)
    {
        case TIMER0:
            g_callBackPtr_Timer0 = a_ptr;
            break;
        case TIMER1:
            g_callBackPtr_Timer1 = a_ptr;
            break;
        case TIMER2:
            g_callBackPtr_Timer2 = a_ptr;
            break;
    }
}
