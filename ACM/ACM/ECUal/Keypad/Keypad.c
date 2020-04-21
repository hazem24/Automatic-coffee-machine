/*
 * Keypad.c
 *
 * Created: 1/3/2020 6:34:10 PM
 *  Author: Mohamed Tag-el-din
 */ 


/*- INCLUDES ----------------------------------------------*/
#include "../../ECUal/KEYPAD/Keypad.h"
#include "../../Mcal/DIO/DIO.h"
#include "../../ECUal/KEYPAD/Keypad_Config.h"
#include "../../ECUal/KEYPAD/Keypad_Retval.h"
/*- LOCAL MACROS ------------------------------------------*/
/*Initiation Flag States*/


/*States for get key state*/
#define CHECK		0u
#define  TAKE_ACTION	1u

/*- LOCAL Data types ----------------------------------------*/
/*- LOCAL FUNCTIONS PROTOTYPES ----------------------------*/
/*- GLOBAL STATIC VARIABLES -------------------------------*/

static u8 Gu8_Init_Flag ;
static u8 Gu8_Get_Key_State;

/*- GLOBAL EXTERN VARIABLES -------------------------------*/
u8 gu8_keypad_buttons_interface[3][3] = 
{
	{'1', '2', '3'},
	{'4', '5', '6'},
	{'7', '8', '9'}
};
/*- LOCAL FUNCTIONS IMPLEMENTATION ------------------------*/
/*- APIs IMPLEMENTATION -----------------------------------*/


extern Error Keypad_Init(void)
{
	Error u8Status = KEYPAD_E_OK;
	
	if (KEYPAD_E_INITIALIZED != Gu8_Init_Flag)
	{
		DIO_Pin_Init(ROW1,OUTPUT);
		DIO_Pin_Init(ROW2,OUTPUT);
		DIO_Pin_Init(ROW3,OUTPUT);
		
		DIO_Pin_Init(COL1,INPUT);
		DIO_Pin_Init(COL2,INPUT);
		DIO_Pin_Init(COL3,INPUT);
		
		DIO_Pin_Activate_pull_up(COL1);
		DIO_Pin_Activate_pull_up(COL2);
		DIO_Pin_Activate_pull_up(COL3);
		
		DIO_Pin_Write(ROW1,HIGH);
		DIO_Pin_Write(ROW2,HIGH);
		DIO_Pin_Write(ROW3,HIGH);
		
		Gu8_Get_Key_State = CHECK;
		Gu8_Init_Flag = KEYPAD_E_INITIALIZED;
	}
	else
	{
		u8Status = KEYPAD_E_INITIALIZED;
	}
	return u8Status;
}

extern Error Keypad_Get_Key(u8* pu8Row,u8* pu8Col)
{
	u8 u8_Detected_Row = NOKEY;
	u8 u8Detected_Col = NOKEY ;
	u8 u8Button_State = HIGH;
	u8 u8Row_index = 0 ;
	Error u8Status = KEYPAD_E_OK;

	*pu8Row = NOKEY ;
	*pu8Col = NOKEY ;
	
	switch (Gu8_Get_Key_State)
	{
		case CHECK:
		{
			DIO_Pin_Toggle(B3);
			
			if (KEYPAD_E_INITIALIZED == Gu8_Init_Flag)
			{
				if ((0 != pu8Row)&&(0 != pu8Col))
				{
					DIO_Pin_Write(ROW1,LOW);
					DIO_Pin_Write(ROW2,HIGH);
					DIO_Pin_Write(ROW3,HIGH);
					
					DIO_Pin_Read(COL1,&u8Button_State);
						
					if (LOW == u8Button_State)
					{
						Gu8_Get_Key_State = TAKE_ACTION;
					}
					else
					{
						DIO_Pin_Read(COL2,&u8Button_State);
							
						if (LOW == u8Button_State)
						{
							Gu8_Get_Key_State = TAKE_ACTION;
						}
						else
						{
							DIO_Pin_Read(COL3,&u8Button_State);
								
							if (LOW == u8Button_State)
							{
								Gu8_Get_Key_State = TAKE_ACTION;
							}
							else
							{
								/*Do Nothing*/
							}
						}
						
					}
					
					DIO_Pin_Write(ROW2,LOW);
					DIO_Pin_Write(ROW1,HIGH);
					DIO_Pin_Write(ROW3,HIGH);
					
					DIO_Pin_Read(COL1,&u8Button_State);
					
					if (LOW == u8Button_State)
					{
						Gu8_Get_Key_State = TAKE_ACTION;
					}
					else
					{
						DIO_Pin_Read(COL2,&u8Button_State);
						if (LOW == u8Button_State)
						{
							Gu8_Get_Key_State = TAKE_ACTION;
						}
						else
						{
							DIO_Pin_Read(COL3,&u8Button_State);
							if (LOW == u8Button_State)
							{
								Gu8_Get_Key_State = TAKE_ACTION;
							}
							else
							{
								/*Do Nothing*/
							}
						}
						
					}
					DIO_Pin_Write(ROW3,LOW);
					DIO_Pin_Write(ROW1,HIGH);
					DIO_Pin_Write(ROW2,HIGH);
					
					DIO_Pin_Read(COL1,&u8Button_State);
					if (LOW == u8Button_State)
					{
						Gu8_Get_Key_State = TAKE_ACTION;
					}
					else
					{
						DIO_Pin_Read(COL2,&u8Button_State);
						if (LOW == u8Button_State)
						{
							Gu8_Get_Key_State = TAKE_ACTION;
						}
						else
						{
							DIO_Pin_Read(COL3,&u8Button_State);
							if (LOW == u8Button_State)
							{
								Gu8_Get_Key_State = TAKE_ACTION;
							}
							else
							{
								/*Do Nothing*/
							}
							DIO_Pin_Write(ROW3,HIGH);
						}
						
					}
				}
				else
				{
					u8Status = KEYPAD_E_NULL_POINTER;
				}
			}
			else
			{
				u8Status = KEYPAD_E_NOT_INITIALIZED_YET;
			}
			break;
		}
		case TAKE_ACTION:
		{
			if (0 == u8Row_index)
			{
				DIO_Pin_Write(ROW1,LOW);
				DIO_Pin_Write(ROW2,HIGH);
				DIO_Pin_Write(ROW3,HIGH);
				
				DIO_Pin_Read(COL1,&u8Button_State);
				
				if (LOW == u8Button_State)
				{
					u8Detected_Col = FIRST_COL;
					u8_Detected_Row = FIRST_ROW;
					Gu8_Get_Key_State = TAKE_ACTION;
				}
				else
				{
					DIO_Pin_Read(COL2,&u8Button_State);
					
					if (LOW == u8Button_State)
					{
						u8Detected_Col = SECOND_COL;
						u8_Detected_Row = FIRST_ROW;
						Gu8_Get_Key_State = TAKE_ACTION;
					}
					else
					{
						DIO_Pin_Read(COL3,&u8Button_State);
						
						if (LOW == u8Button_State)
						{
							u8Detected_Col = THIRD_COL;
							u8_Detected_Row = FIRST_ROW;
							Gu8_Get_Key_State = TAKE_ACTION;
						}
						else
						{
							Gu8_Get_Key_State = CHECK;
							u8Row_index ++ ;
						}
					}
					
				}
			} 
			else
			{
			}
			
			if (1 == u8Row_index)
			{
				DIO_Pin_Write(ROW2,LOW);
				DIO_Pin_Write(ROW1,HIGH);
				DIO_Pin_Write(ROW3,HIGH);
				
				DIO_Pin_Read(COL1,&u8Button_State);
				if (LOW == u8Button_State)
				{
					u8Detected_Col = FIRST_COL;
					u8_Detected_Row = SECOND_ROW;
					Gu8_Get_Key_State = TAKE_ACTION;
				}
				else
				{
					DIO_Pin_Read(COL2,&u8Button_State);
					if (LOW == u8Button_State)
					{
						u8Detected_Col = SECOND_COL;
						u8_Detected_Row = SECOND_ROW;
						Gu8_Get_Key_State = TAKE_ACTION;
					}
					else
					{
						DIO_Pin_Read(COL3,&u8Button_State);
						if (LOW == u8Button_State)
						{
							u8Detected_Col = THIRD_COL;
							u8_Detected_Row = SECOND_ROW;
							Gu8_Get_Key_State = TAKE_ACTION;
						}
						else
						{
							Gu8_Get_Key_State = CHECK;
							u8Row_index ++ ;
						}
					}
					
				}
			} 
			else
			{
			}
			
			if (2 == u8Row_index)
			{
				DIO_Pin_Write(ROW3,LOW);
				DIO_Pin_Write(ROW1,HIGH);
				DIO_Pin_Write(ROW2,HIGH);
				
				DIO_Pin_Read(COL1,&u8Button_State);
				if (LOW == u8Button_State)
				{
					u8Detected_Col = FIRST_COL;
					u8_Detected_Row = THIRD_ROW;
					Gu8_Get_Key_State = TAKE_ACTION;
				}
				else
				{
					DIO_Pin_Read(COL2,&u8Button_State);
					if (LOW == u8Button_State)
					{
						u8Detected_Col = SECOND_COL;
						u8_Detected_Row = THIRD_ROW;
						Gu8_Get_Key_State = TAKE_ACTION;
					}
					else
					{
						DIO_Pin_Read(COL3,&u8Button_State);
						if (LOW == u8Button_State)
						{
							u8Detected_Col = THIRD_COL;
							u8_Detected_Row = THIRD_ROW;
							Gu8_Get_Key_State = TAKE_ACTION;
						}
						else
						{
							Gu8_Get_Key_State = CHECK;
						}
					}
				}
			} 
			else
			{
			}
			
			DIO_Pin_Write(ROW3,HIGH);
			DIO_Pin_Write(ROW1,HIGH);
			DIO_Pin_Write(ROW2,HIGH);
			
			*pu8Row = u8_Detected_Row;
			*pu8Col = u8Detected_Col;
			
			break;
		}
		
		default:
		break;
	}
	return Gu8_Get_Key_State;
}