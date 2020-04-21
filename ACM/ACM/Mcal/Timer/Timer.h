/*
 * Timer.h
 *
 *  Created on: Nov 22, 2019
 *      Author: hosam
 */

#ifndef TIMER_H_
#define TIMER_H_
#include "..\..\Infrastructure\std_types.h"
#include "..\..\Infrastructure\registers.h"
#include "..\..\Infrastructure\common_macros.h"
#include "..\..\Infrastructure\micro_config.h"
#include "..\DIO\dio.h"

#define LED_OFF	0
#define LED_ON	1
#define MAX_NO_OF_TICKS	31
#define TIMER_RESET	0
#define NUMBER_OF_TIMER_CHANNELS 3
#define TIMER_0 0
#define TIMER_1 1
#define TIMER_2 2

#define TIMER_MODE 0
#define COUNTER_MODE 1

#define TIMER_POLLING 0
#define TIMER_INTERRUPT 1

#define TIMER0_OVERFLOW 255
#define TIMER1_OVERFLOW 65536
#define TIMER2_OVERFLOW 255

/*Timer 0 prescallers*/
#define NO_CLOCK_TIMER_0     					0
#define F_CPU_CLOCK_TIMER_0   					1
#define F_CPU_CLOCK_8_TIMER_0 					2
#define F_CPU_CLOCK_64_TIMER_0					3
#define F_CPU_CLOCK_256_TIMER_0					4
#define F_CPU_CLOCK_1024_TIMER_0				5
#define F_EXTERNAL_CLOCK_FALLING_TIMER_0		6
#define F_EXTERNAL_CLOCK_RISING_TIMER_0			7

/*Timer 1 prescallers*/
#define NO_CLOCK_TIMER_1     					0
#define F_CPU_CLOCK_TIMER_1   					1
#define F_CPU_CLOCK_8_TIMER_1 					2
#define F_CPU_CLOCK_64_TIMER_1					3
#define F_CPU_CLOCK_256_TIMER_1					4
#define F_CPU_CLOCK_1024_TIMER_1				5
#define F_EXTERNAL_CLOCK_FALLING_TIMER_1		6
#define F_EXTERNAL_CLOCK_RISING_TIMER_1			7

/*Timer 2 prescallers*/
#define NO_CLOCK_TIMER_2     					0
#define F_CPU_CLOCK_TIMER_2   					1
#define F_CPU_CLOCK_8_TIMER_2 					2
#define F_CPU_CLOCK_32_TIMER_2					3
#define F_CPU_CLOCK_64_TIMER_2					4
#define F_CPU_CLOCK_128_TIMER_2					5
#define F_CPU_CLOCK_256_TIMER_2		 			6
#define F_CPU_CLOCK_1024_TIMER_2				7


typedef struct
{
	uint8_t Channel;
	uint8_t Pre_Scaller;
	uint8_t TimerMode;
	uint8_t Interrupt_Mode;
}Timer_Cfg_S;



uint8_t Timer_Init(Timer_Cfg_S *Timer_CFG);
uint8_t Timer_Start(uint8_t Channel , uint16_t Ticks_Count);
uint8_t Timer_Stop(uint8_t Channel);
uint8_t Timer_Reset(uint8_t Channel);
uint8_t Timer_Get_TickTime(uint8_t Channel, uint32_t *Tick);
uint8_t Timer_Get_FlagStatus(uint8_t Channel,uint8_t *flagstatus);
#endif /* TIMER_H_ */
