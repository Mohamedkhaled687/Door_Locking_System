/******************************************************************************
 *
 * Module: TWI(I2C)
 *
 * File Name: twi.c
 *
 * Description: Source file for the TWI(I2C) AVR driver
 *
 * Author: Mohamed Tarek
 *
 *******************************************************************************/

#include "twi.h"
#include "common_macros.h"
#include <avr/io.h>

void TWI_init(const TWI_ConfigType * Config_Ptr)
{
    /* Set the TWI bit rate based on the provided bit_rate */
    TWBR = Config_Ptr->bit_rate;
    TWSR = 0x00;

    /* Set TWI address */
    TWAR = (Config_Ptr->address << 1); /* Adjust the address to align with the 7-bit addressing */

    /* Enable TWI */
    TWCR = (1 << TWEN);
}

void TWI_start(void)
{
    /* Clear the TWINT flag before sending the start bit, send the start bit, enable TWI Module */
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    
    /* Wait for TWINT flag set in TWCR Register (start bit is sent successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));
}

void TWI_stop(void)
{
    /* Clear the TWINT flag, send the stop bit, enable TWI Module */
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

void TWI_writeByte(uint8 data)
{
    /* Put data on TWI data Register */
    TWDR = data;

    /* Clear the TWINT flag, enable TWI Module */
    TWCR = (1 << TWINT) | (1 << TWEN);

    /* Wait for TWINT flag set in TWCR Register (data is sent successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));
}

uint8 TWI_readByteWithACK(void)
{
    /* Clear the TWINT flag, enable sending ACK, enable TWI Module */
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);

    /* Wait for TWINT flag set in TWCR Register (data received successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));

    /* Read Data */
    return TWDR;
}

uint8 TWI_readByteWithNACK(void)
{
    /* Clear the TWINT flag, enable TWI Module */
    TWCR = (1 << TWINT) | (1 << TWEN);

    /* Wait for TWINT flag set in TWCR Register (data received successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));

    /* Read Data */
    return TWDR;
}

uint8 TWI_getStatus(void)
{
    /* Masking to eliminate first 3 bits and get the last 5 bits (status bits) */
    return TWSR & 0xF8;
}
