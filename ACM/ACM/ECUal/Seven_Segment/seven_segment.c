/*
 * seven_segement.c
 *
 * Created: 13-Apr-20 9:48:10 PM
 *  Author: LENOVO
 */ 

#include "Seven_Segment.h"
#include "../../Mcal/DIO/DIO.h"
#include <avr/io.h>



void Seven_Segment_Init()
{
	DDRD = 0xff;
}

/**
 * @brief Write value inside seven segment.
 * @param au8_port_value 
 */
void Seven_Segment_Write(uint8_t au8_port_value)
{
    PORTD = au8_port_value;
}