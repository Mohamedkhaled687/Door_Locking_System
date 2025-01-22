/******************************************************************************
 *
 * Module: External EEPROM
 *
 * File Name: external_eeprom.c
 *
 * Description: Source file for the External EEPROM Memory
 *
 * Author: Mohamed Tarek
 *
 *******************************************************************************/

#include "external_eeprom.h"   // Include the header file for External EEPROM functions and definitions
#include "twi.h"               // Include the TWI (I2C) communication library
#include <util/delay.h>        // Include delay functions


/*
 * Function: EEPROM_writeByte
 * ---------------------------
 * Writes a single byte of data to a specified address in EEPROM.
 *
 * Parameters:
 *   u16addr - 16-bit address to write to
 *   u8data  - 8-bit data to be written
 *
 * Returns:
 *   SUCCESS if write operation is successful, ERROR otherwise
 */
uint8 EEPROM_writeByte(uint16 u16addr, uint8 u8data)
{
    /* Send the Start Bit for initiating communication */
    TWI_start();
    if (TWI_getStatus() != TWI_START)
        return ERROR;

    /* Send the device address with A8 A9 A10 bits for the memory location and R/W=0 (write mode) */
    TWI_writeByte((uint8)(0xA0 | ((u16addr & 0x0700) >> 7)));
    if (TWI_getStatus() != TWI_MT_SLA_W_ACK)
        return ERROR; 

    /* Send the required memory location address (lower byte of the address) */
    TWI_writeByte((uint8)(u16addr));
    if (TWI_getStatus() != TWI_MT_DATA_ACK)
        return ERROR;

    /* Write the data byte to the specified EEPROM location */
    TWI_writeByte(u8data);
    if (TWI_getStatus() != TWI_MT_DATA_ACK)
        return ERROR;

    /* Send the Stop Bit to terminate the write operation */
    TWI_stop();

    return SUCCESS;
}


/*
 * Function: EEPROM_readByte
 * --------------------------
 * Reads a single byte of data from a specified address in EEPROM.
 *
 * Parameters:
 *   u16addr - 16-bit address to read from
 *   u8data  - Pointer to store the data read from EEPROM
 *
 * Returns:
 *   SUCCESS if read operation is successful, ERROR otherwise
 */
uint8 EEPROM_readByte(uint16 u16addr, uint8 *u8data)
{
    /* Send the Start Bit for initiating communication */
    TWI_start();
    if (TWI_getStatus() != TWI_START)
        return ERROR;

    /* Send the device address with A8 A9 A10 bits and R/W=0 (write mode) to set up read address */
    TWI_writeByte((uint8)(0xA0 | ((u16addr & 0x0700) >> 7)));
    if (TWI_getStatus() != TWI_MT_SLA_W_ACK)
        return ERROR;

    /* Send the required memory location address (lower byte of the address) */
    TWI_writeByte((uint8)(u16addr));
    if (TWI_getStatus() != TWI_MT_DATA_ACK)
        return ERROR;

    /* Send a Repeated Start Bit for reading operation */
    TWI_start();
    if (TWI_getStatus() != TWI_REP_START)
        return ERROR;

    /* Send the device address with A8 A9 A10 bits and R/W=1 (read mode) to read data */
    TWI_writeByte((uint8)(0xA0 | ((u16addr & 0x0700) >> 7) | 1));
    if (TWI_getStatus() != TWI_MT_SLA_R_ACK)
        return ERROR;

    /* Read byte from memory without sending ACK to signal end of reading */
    *u8data = TWI_readByteWithNACK();
    if (TWI_getStatus() != TWI_MR_DATA_NACK)
        return ERROR;

    /* Send the Stop Bit to terminate the read operation */
    TWI_stop();

    return SUCCESS;
}


/*
 * Function: EEPROM_writeArray
 * ----------------------------
 * Writes an array of bytes to EEPROM starting from a specified address.
 *
 * Parameters:
 *   address  - Starting address in EEPROM for the array
 *   arr      - Pointer to the array of data to be written
 *   arr_size - Number of bytes to write from the array
 */
void EEPROM_writeArray(uint16 address, uint8 *arr, uint8 arr_size)
{
    for(uint8 i = 0; i < arr_size; i++) {
        EEPROM_writeByte((address + i), arr[i]);  // Write each byte from the array
        _delay_ms(10);  // Delay to allow EEPROM write time
    }
}


/*
 * Function: EEPROM_readArray
 * ---------------------------
 * Reads an array of bytes from EEPROM starting from a specified address.
 *
 * Parameters:
 *   address  - Starting address in EEPROM for the array
 *   arr      - Pointer to store the read data
 *   arr_size - Number of bytes to read into the array
 */
void EEPROM_readArray(uint16 address, uint8 *arr, uint8 arr_size)
{
    for(uint8 i = 0; i < arr_size; i++) {
        EEPROM_readByte((address + i), &arr[i]);  // Read each byte into the array
        _delay_ms(10);  // Delay to ensure stable reading
    }
}
