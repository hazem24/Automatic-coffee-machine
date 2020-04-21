/*
 * LCD.c
 *
 * Created: 11/16/2019 7:43:36 PM
 *  Author: A.Elkady
 */ 

#include "LCD.h"
#include "../../InfraStructure/micro_config.h"
#include <util/delay.h>
#include <avr/io.h>


/* Static global variable that tells if the LCD module is initialized or not */
static u8 gu8Is_LCD_initialized = LCD_E_NOT_INTIALIZED_YET ;

static u8 gu8LCD_status = LCD_FREE ;

/** 
 * \brief LCD_Init : Initializes the 16x2 LCD 
 *
 * \param *pfSending_Ok  : A pointer to a function to be executed when the sending is done
 * \return void
 *
 **/ 
extern u8 LCD_Init()
{
	/* Static global that holds the Init state */
	static u8 slu8Init_state = 0 ;

	switch(slu8Init_state)
	{
		case (0) :
			DIO_Pin_Init( RW_PIN_NUMBER , OUTPUT);
			DIO_Pin_Init( RS_PIN_NUMBER , OUTPUT);
			DIO_Pin_Init(ENABLE_PIN_NUMBER , OUTPUT);
			
			DIO_Pin_Init( D4_PIN_NUMBER , OUTPUT);
			DIO_Pin_Init( D5_PIN_NUMBER , OUTPUT);
			DIO_Pin_Init( D6_PIN_NUMBER , OUTPUT);
			DIO_Pin_Init( D7_PIN_NUMBER , OUTPUT);
						
			LCD_Send_Command_Stage1(INITIALIZE_COMMAND_1);
			slu8Init_state ++ ;
		break;
		
		case (1) :
			
			LCD_Send_Command_Stage2(INITIALIZE_COMMAND_1);
			slu8Init_state ++ ;
			
		break;
			
		case (2) : 
		
			LCD_Send_Command_Stage1(INITIALIZE_COMMAND_2);
			slu8Init_state ++ ;
			
		break;
			
		case (3) :
			LCD_Send_Command_Stage2(INITIALIZE_COMMAND_2);
			
			slu8Init_state ++ ;
			
		break;
			
		case (4) :
		
			LCD_Send_Command_Stage1(BIT_MODE4);
		
			slu8Init_state ++ ;
		
		break;
		
		case (5) :
		
			LCD_Send_Command_Stage2(BIT_MODE4);
			
			slu8Init_state ++ ;
			
		break;
			
		case (6) :
			LCD_Send_Command_Stage1(DISPLAY_ON_CURSOR_BLINKING);
			
			slu8Init_state ++ ;
			
		break;
			
		case (7) :
		
			LCD_Send_Command_Stage2(DISPLAY_ON_CURSOR_BLINKING);
			
			slu8Init_state ++ ;
			
		break;
			
		case (8) :
		
			LCD_Send_Command_Stage1(CLEAR_DISPLAY);
			
			slu8Init_state ++ ;
			
		break;
			
		case (9) :
		
			LCD_Send_Command_Stage2(CLEAR_DISPLAY);
			
			slu8Init_state ++ ;
			
		break;
		case (10) :
			slu8Init_state ++ ;
			gu8Is_LCD_initialized = LCD_E_INITIALIZED ;
		break;
			
		default:
			
		break;
	}
	return slu8Init_state;
}

/** 
 * \brief LCD_Send_Command_Stage1 : Sends a part of a command to the LCD micro controller
 *
 * \param u8Command(Input) : THe command to be sent 
 * \return void
 *
 **/     
extern void LCD_Send_Command_Stage1(u8 u8Command)
{
	DIO_Pin_Write(ENABLE_PIN_NUMBER , LOW);
	
	LCD_PORT =(LCD_PORT & 0x0F) | (u8Command & 0xF0) ;
	DIO_Pin_Write(RS_PIN_NUMBER, LOW);
	DIO_Pin_Write(RW_PIN_NUMBER , LOW);
	
	DIO_Pin_Write(ENABLE_PIN_NUMBER , HIGH);
	_delay_us(50);
	DIO_Pin_Write(ENABLE_PIN_NUMBER , LOW);
}

/** 
 * \brief LCD_Send_Command_Stage2 : Sends a part of a command to the LCD micro controller
 *
 * \param u8Command(Input) : THe command to be sent 
 * \return void
 *
 **/     
extern void LCD_Send_Command_Stage2(u8 u8Command)
{
	LCD_PORT = u8Command << 4;
	
	DIO_Pin_Write(ENABLE_PIN_NUMBER , HIGH);
	_delay_us(50);
	DIO_Pin_Write(ENABLE_PIN_NUMBER , LOW);
}

/** 
 * \brief LCD_Display_Char_Stage1 : Sends a certain character to the LCD
 *
 * \param u8Data : The data to be sent
 * \return Error : Indication to the function's execution 
 *
 **/     
extern Error LCD_Display_Char_Stage1(char u8Data)
{
	/*
	*	An integer that holds the indication of the execution 
	*/
	Error u8Execution_indication = LCD_E_OK ;
	
	if (LCD_E_INITIALIZED == gu8Is_LCD_initialized)
	{	
		LCD_PORT = (LCD_PORT & 0x0F) | (u8Data & 0xF0) ;
		LCD_PORT |= (1<<RS_PIN_NUMBER);
		
		DIO_Pin_Write(ENABLE_PIN_NUMBER , HIGH);
		_delay_us(50);
		DIO_Pin_Write(ENABLE_PIN_NUMBER , LOW);
	} 
	else
	{
		/* handling of using LCD module before initializing it*/
		u8Execution_indication = LCD_E_NOT_INTIALIZED_YET ;
	}
	return u8Execution_indication ;
}

/** 
 * \brief LCD_Display_Char_Stage2 : Sends a certain character to the LCD
 *
 * \param u8Data : The data to be sent
 * \return Error : Indication to the function's execution 
 *
 **/     
extern Error LCD_Display_Char_Stage2(char u8Data)
{
	/*
	*	An integer that holds the indication of the execution 
	*/
	Error u8Execution_indication = LCD_E_OK ;
	_delay_us(50);
	if (LCD_E_INITIALIZED == gu8Is_LCD_initialized)
	{	
		LCD_PORT = (LCD_PORT & 0x0F) | ((u8Data << 4) & 0xF0) ;
		
		DIO_Pin_Write(ENABLE_PIN_NUMBER , HIGH);
		DIO_Pin_Write(ENABLE_PIN_NUMBER , LOW);
	} 
	else
	{
		/* handling of using LCD module before initializing it*/
		u8Execution_indication = LCD_E_NOT_INTIALIZED_YET ;
	}
	
	return u8Execution_indication ;
}

/** 
 * \brief LCD_Display_Char : Sends a char to the LCD
 *
 * \param u8Data : The data to be sent
 * \return Error : Indication to the function's execution 
 *
 **/     
extern Error LCD_Display_Char(char u8Data)
{
	/*
	*	An integer that holds the indication of the execution 
	*/
	Error u8Execution_indication = LCD_E_OK ;
	
	/*
	*	Static variable that holds the function state 
	*/		
	LCD_Display_Char_Stage1(u8Data);
	_delay_us(100);
	LCD_Display_Char_Stage2(u8Data);
	_delay_us(100);
	return u8Execution_indication ;
}

/** 
 * \brief LCD_Clear
 *
 * \param *pfSending_Ok  : A pointer to a function to be executed when the sending is done
 * \return u8
 *
 **/     
extern u8 LCD_Clear_Display()
{
	/*
	*	An integer that holds the indication of the execution 
	*/
	Error u8Execution_indication = LCD_E_OK ;
	
	/*
	*	Static variable that holds the function state 
	*/
	static u8 slu8Send_state = 0 ;
		
	gu8LCD_status = LCD_BUSY ;	
	if (LCD_E_INITIALIZED == gu8Is_LCD_initialized)
	{
		LCD_Send_Command_Stage1(CLEAR_DISPLAY);
		_delay_us(100);
		LCD_Send_Command_Stage2(CLEAR_DISPLAY);	
	}
	else
	{
		/* handling of using LCD module before initializing it*/
		u8Execution_indication = LCD_E_NOT_INTIALIZED_YET ;
	}
	_delay_us(100);
	return u8Execution_indication ;
}

/** 
 * \brief LCD_Display_String : Sends a string to the LCD
 *
 * \param *pu8Data : a pointer to the data to be sent
 * \param u8Size : The size of the string to be sent 
 * \return Error : Indication to the function's execution 
 *
 **/     
extern Error LCD_Display_String(char *pu8Data , u8 u8Size , pfunc_Sending_Ok_Callback pfSending_Ok)
{
	/*
	*	An integer that holds the indication of the execution 
	*/
	Error u8Execution_indication = LCD_E_OK ;
	
	/*
	*	Static variable that holds the function state 
	*/
	static u8 slu8Send_state = 0 ;
	
	static u8 slu8Count = 0;
	
	gu8LCD_status = LCD_BUSY ;	
	if (LCD_E_INITIALIZED == gu8Is_LCD_initialized)
	{
		if (0 != pu8Data)
		{	
			// 3 CHARS.
			// 0 vs 3 => 1
			// 1 vs 3 => 2
			// 2 vs 3 => 3
 			if (slu8Count < (u8Size))
			{
				LCD_Display_Char(pu8Data[slu8Count]);
				slu8Count++;
			} 
			else
			{
				/* String sending is done */				
				if (pfSending_Ok != 0)
				{
					pfSending_Ok();
					gu8LCD_status = LCD_FREE ;
					slu8Send_state = 0;
					slu8Count = 0 ;
				}
				else
				{
					/* Handling of passing null callback function */
					u8Execution_indication = LCD_E_NULL_POINTER ;
				}
			}
		} 
		else
		{
			/* Handling of sending null pointer */
			u8Execution_indication = LCD_E_NULL_POINTER ;
		}
	}
	else
	{
		/* handling of using LCD module before initializing it*/
		u8Execution_indication = LCD_E_NOT_INTIALIZED_YET ;
	}
	
	return u8Execution_indication ;
}

/** 
 * \brief LCD_Send_Certain_String_xy : Sends a string to the LCD 
 *
 * \param *pu8String : A pointer to the string to be sent
 * \param *pfSending_Ok  : A pointer to a function to be executed when the sending is done
 * \param u8Row  : The desired row
 * \param u8Column  : The desired column
 * \param u8To_clear  : To clear the display or not ( if it's CLEAR it will clear if it's NO_CLEAR it won't)
 * \return Error : Indication to the function's execution
 *
 **/     
extern Error LCD_Send_String_xy(char *pu8String  , pfunc_Sending_Ok_Callback pfSending_Ok , u8 u8Row , u8 u8Column , u8 u8To_clear , u8 u8Size )
{
	/*
	*	An integer that holds the indication of the execution 
	*/
	Error u8Execution_indication = LCD_E_OK ;
	static u8 slu8Address= 0 ;
	
	static u8 slu8Count = 0 ;
	static u8 slu8Send_state = 0 ;
	
	gu8LCD_status = LCD_BUSY ;
	
	if (LCD_E_INITIALIZED == gu8Is_LCD_initialized)
	{
		if (0 != pu8String)
		{
			if (slu8Count < u8Size)
			{
				switch(slu8Send_state)
				{
					case (0) :
					
					if (CLEAR == u8To_clear)
					{
						LCD_Send_Command_Stage1(CLEAR_DISPLAY);
					} 
					else
					{
						/* Do nothing */
					}
					
					slu8Send_state ++ ;
					
					break;
					
					case (1) :
					
					if (CLEAR == u8To_clear)
					{
						LCD_Send_Command_Stage2(CLEAR_DISPLAY);
					}
					else
					{
						/* Do nothing */
					}
					
					
					slu8Send_state ++ ;
					
					break;
					
					case (2) :
					switch(u8Row)
					{
						case 1:
						slu8Address = 0x80 + u8Column;
						break;
						
						case 2:
						slu8Address = 0xC0 + u8Column;
						break;
						
						default:
						break;
					}
					
					LCD_Send_Command_Stage1(slu8Address | 0b100000000);
					slu8Send_state ++ ;
					break;
					
					case (3) :
					
					LCD_Send_Command_Stage2(slu8Address | 0b100000000);
					slu8Send_state ++ ;
					break;
					
					
					case (4) :
					
					LCD_Display_Char_Stage1(pu8String[slu8Count]);
					slu8Send_state ++ ;
					break;
					
					case (5) :
					
					LCD_Display_Char_Stage2(pu8String[slu8Count]);
					slu8Send_state = 4 ;
					slu8Count ++ ;
					break;
				}
			}
			else
			{
				/* String sending is done */
				slu8Count = 0 ;
				
				gu8LCD_status = LCD_FREE ;
				slu8Address = 0;
				slu8Send_state = 0;
				if (pfSending_Ok != 0)
				{
					pfSending_Ok();
				} 
				else
				{
					/* Handling of passing null callback function */
					u8Execution_indication = LCD_E_NULL_POINTER ;
				}
				
			}
			
		} 
		else
		{
			/* Handling of passing a null pointer */
			u8Execution_indication = LCD_E_NULL_POINTER ;
		}
		
	}
	else
	{
		/* handling of using LCD module before initializing it*/
		u8Execution_indication = LCD_E_NOT_INTIALIZED_YET ;
	}
	return u8Execution_indication;
}

/** 
 * \brief Get_LCD_Status : Returns the status of the LCD module
 *
 * \param void
 * \return u8 : The status of the LCD module
 *
 **/     
extern u8 Get_LCD_Status(void)
{
	return gu8LCD_status ;
}

/** 
 * \brief LCD_Send_Certain_String_xy : Sends a certain string to the LCD (e.g. "Hello") on a certain position
 *
 * \param *pu8String : A pointer to the string to be sent
 * \param *pfSending_Ok  : A pointer to a function to be executed when the sending is done
 * \param u8Row  : The desired row
 * \param u8Column  : The desired column
 * \param u8To_clear  : To clear the display or not ( if it's CLEAR it will clear if it's NO_CLEAR it won't)
 * \return Error : Indication to the function's execution
 *
 **/     
extern Error LCD_Send_Certain_String_xy(char *pu8String  , pfunc_Sending_Ok_Callback pfSending_Ok , u8 u8Row , u8 u8Column , u8 u8To_clear)
{
	/*
	*	An integer that holds the indication of the execution 
	*/
	Error u8Execution_indication = LCD_E_OK ;
	static u8 slu8Address= 0 ;
	
	static u8 slu8Count = 0 ;
	static u8 slu8Send_state = 0 ;
	
	gu8LCD_status = LCD_BUSY ;
	
	if (LCD_E_INITIALIZED == gu8Is_LCD_initialized)
	{
		if (0 != pu8String)
		{
			if (pu8String[slu8Count] != 0)
			{
				switch(slu8Send_state)
				{
					case (0) :
					
					if (CLEAR == u8To_clear)
					{
						LCD_Send_Command_Stage1(CLEAR_DISPLAY);
					} 
					else
					{
						/* Do nothing */
					}
					
					slu8Send_state ++ ;
					
					break;
					
					case (1) :
					
					if (CLEAR == u8To_clear)
					{
						LCD_Send_Command_Stage2(CLEAR_DISPLAY);
					}
					else
					{
						/* Do nothing */
					}
					
					
					slu8Send_state ++ ;
					
					break;
					
					case (2) :
					switch(u8Row)
					{
						case 1:
						slu8Address = 0x80 + u8Column;
						break;
						
						case 2:
						slu8Address = 0xC0 + u8Column;
						break;
						
						default:
						break;
					}
					
					LCD_Send_Command_Stage1(slu8Address | 0b100000000);
					slu8Send_state ++ ;
					break;
					
					case (3) :
					
					LCD_Send_Command_Stage2(slu8Address | 0b100000000);
					slu8Send_state ++ ;
					break;
					
					
					case (4) :
					
					LCD_Display_Char_Stage1(pu8String[slu8Count]);
					slu8Send_state ++ ;
					break;
					
					case (5) :
					
					LCD_Display_Char_Stage2(pu8String[slu8Count]);
					slu8Send_state = 4 ;
					slu8Count ++ ;
					break;
				}
			}
			else
			{
				/* String sending is done */
				slu8Count = 0 ;
				
				gu8LCD_status = LCD_FREE ;
				slu8Address = 0;
				slu8Send_state = 0;
				
				if (pfSending_Ok != 0)
				{
					pfSending_Ok();
				} 
				else
				{
					/* Handling of passing null callback function */
					u8Execution_indication = LCD_E_NULL_POINTER ;
				}
				
			}
			
		} 
		else
		{
			/* Handling of passing a null pointer */
			u8Execution_indication = LCD_E_NULL_POINTER ;
		}
		
	}
	else
	{
		/* handling of using LCD module before initializing it*/
		u8Execution_indication = LCD_E_NOT_INTIALIZED_YET ;
	}
	return u8Execution_indication;
}

extern Error LCD_Goto(u8 row, u8 col)
{
	
	uint8_t address;
	switch(row)
	{
		case 0:
		address = 0x80 + col - 1;
		//OR LCD_sendCommand(0x80).
		break;
		case 1:
		address = 0xC0 + col -1 ;
		//OR LCD_sendCommand(0xC0);
		break;
		case 2:
		address = 0x94 + col - 1;
		//OR LCD_sendCommand(0xC0);
		break;
		case 3:
		address = 0xD4 + col - 1;
		//OR LCD_sendCommand(0xC0);
		break;
		default:
		break;
		//Nothing.
	}
	LCD_Send_Command_Stage1(address | 0b100000000);
	_delay_us(150);
	LCD_Send_Command_Stage2(address | 0b100000000);
	_delay_us(100);
	return 0;
}
