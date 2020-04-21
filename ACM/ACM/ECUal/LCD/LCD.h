/*
 * LCD.h
 *
 * Created: 11/16/2019 7:43:49 PM
 *  Author: A.Elkady
 */ 


#ifndef LCD_H_
#define LCD_H_

#include "../../InfraStructure/micro_config.h"
#include "../../InfraStructure/Data_Types.h"
#include "../../MCAL/DIO/DIO.h"
#include "LCD_Ret_Val.h"

/* Used macros */
#define RW_WRITE 0
#define RW_READ 1

#define RS_COMMAND 0
#define RS_DATA 1

#define ENABLE_LOW 0
#define ENABLE_HIGH 1

#define MAX_LENGTH 16

#define LCD_FREE 0u
#define LCD_BUSY 1u

#define CLEAR    1u
#define NO_CLEAR 0u
/* LCD Pins definitions */
#define LCD_PORT PORT_A

#define D4_PIN_NUMBER      A4
#define D5_PIN_NUMBER      A5
#define D6_PIN_NUMBER      A6
#define D7_PIN_NUMBER      A7

#define RS_PIN_NUMBER      A1
#define RW_PIN_NUMBER      A2
#define ENABLE_PIN_NUMBER  A3

/* COMMANDS */
#define CLEAR_DISPLAY 0x01

#define SHIFT_CURSOR_LEFT 0x04
#define SHIFT_CURSOR_RIGHT 0x06

#define SHIFT_CURSOR_POSITION_LEFT 0x10
#define SHIFT_CURSOR_POSITION_RIGHT 0x14

#define DISPLAY_ON_CURSOR_OFF 0x0c
#define DISPLAY_ON_CURSOR_NOT_BLINKING 0x0e
#define DISPLAY_ON_CURSOR_BLINKING 0x0f

#define BIT_MODE4 0x28
#define INITIALIZE_COMMAND_1 0x33
#define INITIALIZE_COMMAND_2 0x32

#define SHIFT_DISPLAY_RIGHT 0x1c
#define SHIFT_DISPLAY_LEFT 0x18

typedef u8 (*pfunc_Sending_Ok_Callback)(void) ;

/** 
 * \brief LCD_Init : Initializes the 16x2 LCD 
 *
 * \param *pfSending_Ok  : A pointer to a function to be executed when the sending is done
 * \return u8
 *
 **/     
extern u8 LCD_Init();

/** 
 * \brief LCD_Clear
 *
 * \param *pfSending_Ok  : A pointer to a function to be executed when the sending is done
 * \return u8
 *
 **/     
extern u8 LCD_Clear(pfunc_Sending_Ok_Callback pfSending_Ok);
/** 
 * \brief LCD_Send_Command_Stage1 : Sends a part of a command to the LCD micro controller
 *
 * \param u8Command(Input) : THe command to be sent 
 * \return void
 *
 **/     
extern void LCD_Send_Command_Stage1(u8 u8Command);

/** 
 * \brief LCD_Send_Command_Stage2 : Sends a part of a command to the LCD micro controller
 *
 * \param u8Command(Input) : The command to be sent 
 * \return void
 *
 **/     
extern void LCD_Send_Command_Stage2(u8 u8Command);

/** 
 * \brief LCD_Display_Char_Stage1 : Sends a certain character to the LCD
 *
 * \param u8Data : The data to be sent
 * \return Error : Indication to the function's execution 
 *
 **/     
extern Error LCD_Display_Char_Stage1(char u8Data);

/** 
 * \brief LCD_Display_Char : Sends a char to the LCD
 *
 * \param u8Data : The data to be sent
 * \return Error : Indication to the function's execution 
 *
 **/     
extern Error LCD_Display_Char(char u8Data);

/** 
 * \brief LCD_Display_Char_Stage2 : Sends a certain character to the LCD
 *
 * \param u8Data : The data to be sent
 * \return Error : Indication to the function's execution 
 *
 **/     
extern Error LCD_Display_Char_Stage2(char u8Data);

/** 
 * \brief LCD_Display_String : Sends a string to the LCD
 *
 * \param *pu8Data : a pointer to the data to be sent
 * \param u8Size : The size of the string to be sent 
 * \param *pfSending_Ok  : A pointer to a function to be executed when the sending is done
 * \return Error : Indication to the function's execution 
 *
 **/     
extern Error LCD_Display_String(char *pu8Data , u8 u8Size , pfunc_Sending_Ok_Callback pfSending_Ok);

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
extern Error LCD_Send_String_xy(char *pu8String  , pfunc_Sending_Ok_Callback pfSending_Ok , u8 u8Row , u8 u8Column , u8 u8To_clear , u8 u8Size);


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
extern Error LCD_Send_Certain_String_xy(char *pu8String  , pfunc_Sending_Ok_Callback pfSending_Ok , u8 u8Row , u8 u8Column , u8 u8To_clear);

/** 
 * \brief LCD_Goto : Moves the cursor to a certain co-ordinations
 *
 * \param u8Row : The desired row
 * \param u8Column : The desired column 
 * \return Error : Indication to the function's execution
 *
 **/     
extern Error LCD_Goto(u8 u8Row, u8 u8Column);

/** 
 * \brief LCD_Clear_Display : Clears the display 
 *
 * \param void
 * \return void
 *
 **/     
extern Error LCD_Clear_Display(void);

/** 
 * \brief Get_LCD_Status : Returns the status of the LCD module
 *
 * \param void
 * \return u8 : The status of the LCD module
 *
 **/     
extern u8 Get_LCD_Status(void);

#endif /* LCD_H_ */