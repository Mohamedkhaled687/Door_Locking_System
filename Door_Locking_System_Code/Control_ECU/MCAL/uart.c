 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.c
 *
 * Description: Source file for the UART AVR driver
 *
 * Author: Mohamed Tarek
 *
 *******************************************************************************/

#include "uart.h"
#include "avr/io.h" /* To use the UART Registers */
#include "common_macros.h" /* To use the macros like SET_BIT */

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(UART_ConfigType * Config_Ptr)
{
    uint16 ubrr_value = 0;

    /* U2X = 1 for double transmission speed */
    UCSRA = (1 << U2X);

    /* Enable the Sending and Receiving enable*/
    UCSRB = (1 << RXEN) | (1 << TXEN);

    /* If 9-bit data size, enable UCSZ2 in UCSRB */
    if (Config_Ptr->bit_data == Character_SIZE_9)
    {
        UCSRB |= (1 << UCSZ2); /* Set UCSZ2 for 9-bit data size */
    }
    else
    {
        UCSRB &= ~(1 << UCSZ2); /* Clear UCSZ2 for other data sizes */
    }

    /************************** UCSRC Description **************************
     * URSEL  = 1  to select the UCSRC register
     * Set UCSZ1 and UCSZ0 for character size based on Config_Ptr->bit_data
     ***********************************************************************/
    UCSRC = (1 << URSEL);  /* To select UCSRC register */

    /* Write the Parity bit (Config_Ptr->parity) */
    UCSRC |= (Config_Ptr->parity);

    /* Write the Stop bit (Config_Ptr->stop_bit) */
    UCSRC |= (Config_Ptr->stop_bit);

    /* Write the character size (UCSZ1, UCSZ0 in UCSRC) */
    UCSRC |= ((Config_Ptr->bit_data & 0x03) << 1); /* Shift left to align UCSZ0 and UCSZ1 */

    /* Calculate the UBRR value for the baud rate */
    ubrr_value = (uint16)(((F_CPU / (Config_Ptr->baud_rate * 8UL))) - 1);

    /* Set the UBRR value: First 8 bits in UBRRL and the higher 4 bits in UBRRH */
    UBRRH = (ubrr_value >> 8);
    UBRRL = ubrr_value;
}

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data)
{
	/*
	 * UDRE flag is set when the Tx buffer (UDR) is empty and ready for
	 * transmitting a new byte so wait until this flag is set to one
	 */
	while(BIT_IS_CLEAR(UCSRA,UDRE)){}

	/*
	 * Put the required data in the UDR register and it also clear the UDRE flag as
	 * the UDR register is not empty now
	 */
	UDR = data;

	/************************* Another Method *************************
	UDR = data;
	while(BIT_IS_CLEAR(UCSRA,TXC)){} // Wait until the transmission is complete TXC = 1
	SET_BIT(UCSRA,TXC); // Clear the TXC flag
	*******************************************************************/
}

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void)
{
	/* RXC flag is set when the UART receive data so wait until this flag is set to one */
	while(BIT_IS_CLEAR(UCSRA,RXC)){}

	/*
	 * Read the received data from the Rx buffer (UDR)
	 * The RXC flag will be cleared after read the data
	 */
    return UDR;		
}

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str)
{
	uint8 i = 0;

	/* Send the whole string */
	while(Str[i] != '\0')
	{
		UART_sendByte(Str[i]);
		i++;
	}
	/************************* Another Method *************************
	while(*Str != '\0')
	{
		UART_sendByte(*Str);
		Str++;
	}		
	*******************************************************************/
}

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str)
{
	uint8 i = 0;

	/* Receive the first byte */
	Str[i] = UART_recieveByte();

	/* Receive the whole string until the '#' */
	while(Str[i] != '#')
	{
		i++;
		Str[i] = UART_recieveByte();
	}

	/* After receiving the whole string plus the '#', replace the '#' with '\0' */
	Str[i] = '\0';
}
