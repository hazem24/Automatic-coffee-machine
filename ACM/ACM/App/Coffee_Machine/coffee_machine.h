/*
 * coffee_machine.h
 *
 * Created: 14-Apr-20 2:19:54 AM
 *  Author: LENOVO
 */ 


#ifndef COFFEE_MACHINE_H_
#define COFFEE_MACHINE_H_


/*- INCLUDES -----------------------------------------------*/
#include "../../InfraStructure/std_types.h"

/*- CONSTANTS ----------------------------------------------*/
/*- PRIMITIVE TYPES ----------------------------------------*/
/*- ENUMS --------------------------------------------------*/
/*- STRUCTS AND UNIONS -------------------------------------*/
/*- FUNCTION-LIKE MACROS -----------------------------------*/
/*- FUNCTION DECLARATIONS ----------------------------------*/


/* Handler for some tasks. */
/* Handler for collect coins task. */
extern TaskHandle_t gu8_collect_coins_handle;
/* Handler for read cmd task. */
extern TaskHandle_t gu8_read_cmd_handle;
/* Handler for display_task. */
extern TaskHandle_t gu8_display_handle;

extern TaskHandle_t gu8_give_refund;
/*-- End for handlers. --*/

/**
 * @brief Task to => Init all Modules needed.
 * @param a_ptr 
 */
void System_Init_Task(void* a_ptr);


/**
 * @brief This Task Responsable to collect coins from the user.
 * @priority 5 => First Priority in the system.
 * @param a_ptr 
 */
void Collect_Coins_Task( void* a_ptr );

/**
 * @brief This Task Responseable to take cmds from user via Keypad.
 * @priority 4 => Second Priority in the system.
 * @param a_ptr 
 */
void Read_Cmd_Task( void* a_ptr );

/**
 * @brief Deliver The Choosen Coffee For the user.
 * @priority 3 => Third Priority in the system.
 * @param a_ptr 
 */
void Deliver_Coffee_Task( void* a_ptr );

/**
 * @brief Give Refund For the user in two cases.
    * if the input coins < the price of the coffee.
    * if the user put coins > cofee price so the machine should return the differnce between it.
 * @priority 2 => Fourth Priority in the system.
 * @param a_ptr
 */
void Give_Refund_Task( void* a_ptr );

/** @brief This Task Responsiable to display messages to user.
 * @priority 1 => Lowest Priority in the system.
 * @param au8_ptr 
 */
void Display_Messages_Task( void* a_ptr );



#endif /* COFFEE_MACHINE_H_ */