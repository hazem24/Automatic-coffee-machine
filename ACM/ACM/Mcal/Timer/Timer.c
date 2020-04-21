/*
 * Timer.c
 *
 *  Created on: Nov 22, 2019
 *      Author: hosam
 */



#include "Timer.h"
/* __vector_10 for timer 0 and __vector_4 for timer 2 in Atmega32*/


/* static global array to save the Pre_scallers values*/
static uint8_t Pre_scaller[NUMBER_OF_TIMER_CHANNELS]= {0};

/* static global array to save the tick time values in nano seconds*/
static uint32_t TickTime[NUMBER_OF_TIMER_CHANNELS] = {0};

/* static global array to save if mode was pooling or interrupt*/
static uint8_t Int[3] = {NUMBER_OF_TIMER_CHANNELS};

/***************************************************************************
 * Function: Timer_Init
 * @param: *Timer_CFG, a pointer to as structure sent by the user
 * Description: Function to initialize Timers and return Status
 **************************************************************************/


uint8_t Timer_Init(Timer_Cfg_S *Timer_CFG)
{
	uint8_t state;
	/*Timer 0*/
	if (TIMER_0 == Timer_CFG->Channel)
	{
		Pre_scaller[Timer_CFG->Channel]= Timer_CFG->Pre_Scaller;
		TickTime[Timer_CFG->Channel] = (Pre_scaller[Timer_CFG->Channel]/F_CPU)*1000000000UL;
		Int[Timer_CFG->Channel] = Timer_CFG->Interrupt_Mode;
		SET_BIT(TCCR0,FOC0);
		SET_BIT(TCCR0,WGM01);
		CLEAR_BIT(TCCR0,WGM00);
		/* Timer Mode*/
		if (TIMER_MODE == Timer_CFG->TimerMode)
		{
			/* polling Mode*/
			if (TIMER_POLLING == Timer_CFG->Interrupt_Mode)
			{
				TCCR0 = (TCCR0&0xF8) | (Pre_scaller[Timer_CFG->Channel]);
				state = ERROR_OK;
			}
			/*interrupt mode*/
			else if (TIMER_INTERRUPT == Timer_CFG->Interrupt_Mode)
			{
				TCCR0 = (TCCR0&0xF8) | (Pre_scaller[Timer_CFG->Channel]);
				SET_BIT(TIMSK,OCIE0);
				SET_BIT(SREG,I);
				state = ERROR_OK;
			}
			else
			{
				state = ERROR_NOK;
			}
		}
		/*Counter Mode*/
		else if (COUNTER_MODE == Timer_CFG->TimerMode)
		{
			state = ERROR_OK;
		}
		else
		{
			state = ERROR_NOK;
		}
	}

	/* Timer 1*/
	else if (TIMER_1 == Timer_CFG->Channel)
	{
		Pre_scaller[Timer_CFG->Channel]= Timer_CFG->Pre_Scaller;
		TickTime[Timer_CFG->Channel] = (Pre_scaller[Timer_CFG->Channel]/F_CPU)*1000000000UL;
		Int[Timer_CFG->Channel] = Timer_CFG->Interrupt_Mode;
		SET_BIT(TCCR1A,FOC1A);
		SET_BIT(TCCR1A,FOC1B);
		SET_BIT(TCCR1B,WGM12);
		CLEAR_BIT(TCCR1A,WGM10);
		CLEAR_BIT(TCCR1A,WGM11);
		CLEAR_BIT(TCCR1B,WGM13);
		if (TIMER_POLLING == Timer_CFG->Interrupt_Mode)
		{
			TCCR1B = (TCCR1B&0xF8) | (Pre_scaller[Timer_CFG->Channel]);
			state = ERROR_OK;
		}
		else if (TIMER_INTERRUPT == Timer_CFG->Interrupt_Mode)
		{
			TCCR1B = (TCCR1B&0xF8) | (Pre_scaller[Timer_CFG->Channel]);
			SET_BIT(TIMSK,OCIE1A);
			SET_BIT(SREG,I);
			state = ERROR_OK;
		}
		else
		{
			state = ERROR_NOK;
		}
	}

	/*Timer 2 */
	else if (TIMER_2 == Timer_CFG->Channel)
	{
		Pre_scaller[Timer_CFG->Channel]= Timer_CFG->Pre_Scaller;
		TickTime[Timer_CFG->Channel] = (Pre_scaller[Timer_CFG->Channel]/F_CPU)*1000000000UL;
		Int[Timer_CFG->Channel] = Timer_CFG->Interrupt_Mode;
		SET_BIT(TCCR2,FOC2);
		SET_BIT(TCCR2,WGM21);
		CLEAR_BIT(TCCR2,WGM20);
		/*polling mode*/
		if (TIMER_POLLING == Timer_CFG->Interrupt_Mode)
		{
			TCCR2 = (TCCR2&0xF8) | (Pre_scaller[Timer_CFG->Channel]);
			state = ERROR_OK;
		}
		/*interrupt mode*/
		else if (TIMER_INTERRUPT == Timer_CFG->Interrupt_Mode)
		{
			TCCR2 = (TCCR2&0xF8) | (Pre_scaller[Timer_CFG->Channel]);
			SET_BIT(TIMSK,OCIE2);
			SET_BIT(SREG,I);
			state = ERROR_OK;
		}
		else
		{
			state = ERROR_NOK;
		}
	}
	else
	{
		state = ERROR_NOK;
	}
	return state;
}

/***************************************************************************
 * Function: Timer_Start
 * @param: Channel, Timer used number sent by the user
 * @param: Timercount, Timer value needed to be count sent by the user
 * Description: Function to start the counting process and return status
 **************************************************************************/

uint8_t Timer_Start(uint8_t Channel , uint16_t Ticks_Count)
{
	uint8_t status;
	if (TIMER_0 == Channel)
	{
		if(TIMER_POLLING == Int[Channel])
		{
			if ((Ticks_Count <= TIMER0_OVERFLOW) && (Ticks_Count >=LOW))
			{
				OCR0=Ticks_Count;
				SET_BIT(TIFR,OCF0);
				TCNT0 = 0;
				status = ERROR_OK;
			}
			else
			{
				status = ERROR_NOK;
			}
		}
		else if (TIMER_INTERRUPT == Int[Channel])
		{
			if ((Ticks_Count <= TIMER0_OVERFLOW) && (Ticks_Count >=LOW))
			{
				OCR0=Ticks_Count;
				TCNT0 = 0;
				status = ERROR_OK;
			}
			else
			{
				status = ERROR_NOK;
			}
		}
	}
	else if (TIMER_1 == Channel)
	{
		if(TIMER_POLLING == Int[Channel])
		{
			if ((Ticks_Count <= TIMER1_OVERFLOW) && (Ticks_Count >=LOW))
			{
				OCR1AH = (Ticks_Count>>8);
				OCR1AL= Ticks_Count;
				SET_BIT(TIFR,OCF1A);
				TCNT1H=0;
				TCNT1L=0;
				status = ERROR_OK;
			}
			else
			{
				status = ERROR_NOK;
			}
		}
		else if (TIMER_INTERRUPT == Int[Channel])
		{
			if ((Ticks_Count <= TIMER1_OVERFLOW) && (Ticks_Count >=LOW))
			{
				OCR1AH = (Ticks_Count>>8);
				OCR1AL= Ticks_Count;
				TCNT1H=0;
				TCNT1L=0;
				status = ERROR_OK;
			}
			else
			{
				status = ERROR_NOK;
			}
		}
	}
	else if (TIMER_2 == Channel)
	{
		if(TIMER_POLLING == Int[Channel])
		{
			if ((Ticks_Count <= TIMER2_OVERFLOW) && (Ticks_Count >=LOW))
			{
				OCR2=Ticks_Count;
				SET_BIT(TIFR,OCF2);
				TCNT2 = 0;
				status = ERROR_OK;
			}
			else
			{
				status = ERROR_NOK;
			}
		}
		else if (TIMER_INTERRUPT == Int[Channel])
		{
			if ((Ticks_Count <= TIMER2_OVERFLOW) && (Ticks_Count >=LOW))
			{
				OCR2=Ticks_Count;
				TCNT2 = 0;
				status = ERROR_OK;
			}
			else
			{
				status = ERROR_NOK;
			}
		}
	}
	else
	{
		status = ERROR_NOK;
	}
	return status;
}

/***************************************************************************
 * Function: Timer_Stop
 * @param: Channel, Timer used number sent by the user
 * Description: Function to stop the counting process
 * and return status
 **************************************************************************/

uint8_t Timer_Stop(uint8_t Channel)
{
	uint8_t status;
	if (TIMER_0 == Channel)
	{
		TCNT0 = 0;
		TCCR0 =0;
		CLEAR_BIT(TIMSK,OCIE0);
		status = ERROR_OK;
	}
	else if (TIMER_1 == Channel)
	{
		TCNT1H = 0;
		TCNT1L =0;
		TCCR1A = 0;
		TCCR1B=0;
		status = ERROR_OK;
	}
	else if (TIMER_2 == Channel)
	{
		TCNT2 = 0;
		TCCR2=0;
		CLEAR_BIT(TIMSK,OCIE2);
		status= ERROR_OK;
	}
	else {
		status = ERROR_NOK;
	}
	return status;
}

/***************************************************************************
 * Function: Timer_Reset
 * @param: Channel, Timer used number sent by the user
 * Description: Function to reset the counting process
 * and return status
 **************************************************************************/
uint8_t Timer_Reset(uint8_t Channel)
{
	uint8_t status;
	if (TIMER_0 == Channel)
	{
		TCNT0 = 0;
		status = ERROR_OK;
	}
	/*else if (TIMER_1 == Channel)
	{
		TCNT1H = 0;
		TCNT1L =0;
		status= ERROR_OK;
	}*/
	else if (TIMER_2 == Channel)
	{
		TCNT2 = 0;
		status= ERROR_OK;
	}
	else
	{
		status = ERROR_NOK;
	}
	return status;
}

/***************************************************************************
 * Function: Timer_Get_TickTime
 * @param: Channel, Timer used number sent by the user
 * @param: *Tick, variable to store TickTime in nano seconds
 * Description: Function to reset the counting process
 * and return status
 **************************************************************************/

uint8_t Timer_Get_TickTime(uint8_t Channel, uint32_t *Tick)
{
	uint8_t status;
	if (Tick == NULL_PTR)
	{
		status = ERROR_NOK;
	}
	else
	{
		if (TIMER_0 == Channel)
		{
			*Tick = TickTime[Channel];
			status = ERROR_OK;
		}
		/*else if (TIMER_1 == Channel)
	{
		 *Tick = TickTime[Channel];
		status = ERROR_OK;
	}*/
		else if (TIMER_2 == Channel)
		{
			*Tick = TickTime[Channel];
			status = ERROR_OK;
		}
		else
		{
			status= ERROR_NOK;
		}
	}
	return status;
}

/***************************************************************************
 * Function: Timer_Get_Flag
 * @param: Channel, Timer used number sent by the user
 * @param: *Flagstatus, variable to store Flag value in
 * Description: Function to return flag value and return status
 **************************************************************************/

uint8_t Timer_Get_FlagStatus(uint8_t Channel,uint8_t *flagstatus)
{
	uint8_t status;
	if (flagstatus == NULL_PTR)
	{
		status = ERROR_NOK;
	}
	else
	{
		if (TIMER_0 == Channel)
		{
			*flagstatus = GET_BIT(TIFR,OCF0);
			status = ERROR_OK;
		}
		/*else if (TIMER_1 == Channel)
			{

				status = ERROR_OK;
			}*/
		else if (TIMER_2 == Channel)
		{
			*flagstatus = GET_BIT(TIFR,OCF2);
			status = ERROR_OK;
		}
		else
		{
			status= ERROR_NOK;
		}
	}
	return status;
}

