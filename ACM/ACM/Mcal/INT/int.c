/*
 * INT.c
 *
 *  Created on: Jan 13, 2020
 *      Author: hosam
 */
#include "int.h"

void __vector_1(void)__attribute((signal,used,externally_visible));
void __vector_2(void)__attribute((signal,used,externally_visible));
void __vector_3(void)__attribute((signal,used,externally_visible));


void INT_Init(INT_CFG_S *INT_cfg)
{
	CLEAR_BIT(SREG,7);
	if(INT_cfg->u8_int_channel == INT_0)
	{
		DIO_Pin_Init(D2,LOW);
		GICR|=(1<<INT0);
		MCUCR = (MCUCR&0xFC) |INT_cfg->u8_int_sense;
	}
	else if(INT_cfg->u8_int_channel == INT_1)
	{
		DIO_Pin_Init(D3,LOW);
		GICR|=(1<<INT1);
		MCUCR = (MCUCR&0xF3) |INT_cfg->u8_int_sense;

	}
	else if(INT_cfg->u8_int_channel == INT_2)
	{
		DIO_Pin_Init(B2,LOW);
		GICR|=(1<<INT2);
		MCUCSR = (MCUCSR&0xBF) |INT_cfg->u8_int_sense;
	}
	else
	{

	}
	SET_BIT(SREG,7);
}

void INT_Deinit(uint8_t Channel)
{
	CLEAR_BIT(SREG,7);
	if (Channel == INT_0)
	{
		GICR&=~(1<<INT0);
	}
	else if(Channel == INT_1)
	{
		GICR&=~(1<<INT1);
	}
	else if(Channel == INT_2)
	{
		GICR&=~(1<<INT2);
	}
	else
	{

	}
}
