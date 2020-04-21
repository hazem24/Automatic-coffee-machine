/*
 * Keypad.h
 *
 * Created: 1/3/2020 6:34:37 PM
 *  Author: Mohamed Tag-el-din
 */ 


#ifndef KEYPAD_H_
#define KEYPAD_H_

#include "../../InfraStructure/micro_config.h"
#include "../../InfraStructure/Data_Types.h"
#include <util/delay.h>


extern u8 gu8_keypad_buttons_interface[][3];
/*Detected_u8pu8Row_&_pu8Colum_states*/
#define NOKEY		0
#define FIRST_ROW	1
#define SECOND_ROW	2
#define THIRD_ROW	3
#define FIRST_COL	1
#define SECOND_COL	2
#define THIRD_COL	3


extern Error Keypad_Init(void);
extern Error Keypad_Get_Key(u8* u8pu8Row,u8* pu8Col);

#endif /* KEYPAD_H_ */