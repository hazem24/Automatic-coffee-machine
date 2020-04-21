/*
 * INT.h
 *
 *  Created on: Jan 13, 2020
 *      Author: hosam
 */

#ifndef MCAL_INT_INT_H_
#define MCAL_INT_INT_H_

#include "../../Infrastructure/std_types.h"
#include <avr/interrupt.h>
#include "../../Infrastructure/common_macros.h"
#include "../../Infrastructure/micro_config.h"
#include "../DIO/DIO.h"


#define INT_0	0
#define INT_1	1
#define INT_2	2

#define INT0_LOW_LEVEL		0
#define INT0_ANY_CHANGE		1
#define INT0_FALLING_EDGE	2
#define INT0_RISING_EDGE	3

#define INT1_LOW_LEVEL		0
#define INT1_ANY_CHANGE		1
#define INT1_FALLING_EDGE	2
#define INT1_RISING_EDGE	3

#define INT2_FALLING_EDGE	0
#define INT2_RISING_EDGE	1


#define DISABLE_INTERRUPT()    cli()

#define ENABLE_INTERRUPT()     sei()


typedef struct
{
	uint8_t u8_int_channel;
	uint8_t u8_int_sense;

}INT_CFG_S;
void INT_Init(INT_CFG_S *INT_cfg);

void INT_Deinit(uint8_t Channel);

#endif /* MCAL_INT_INT_H_ */
