/*
 * ACM.c
 *
 * Created: 13-Apr-20 8:59:38 PM
 * Author : LENOVO
 */ 

#include "InfraStructure/std_types.h"
#include "Service/FreeRtos/FreeRTOS.h"
#include "Service/FreeRtos/task.h"
#include "Service/FreeRtos/queue.h"
#include "App/Coffee_Machine/coffee_machine.h"

/* Priority Table. */
#define FIRST_PRIORITY    (uint8_t) 4
#define SECOND_PRIORITY   (uint8_t) 3
#define THIRD_PRIORITY    (uint8_t) 3
#define FOURTH_PRIORITY   (uint8_t) 2
#define FIFTH_PRIORITY    (uint8_t) 1
#define SIXTH_PRIORITY    (uint8_t) 1


int main(void)
{
    /* Create Tasks. */
    /* Creation of init Task. */
    xTaskCreate (   System_Init_Task,
                    NULL,
                    100,
                    NULL,
                    FIRST_PRIORITY,
                    NULL
                );

    xTaskCreate (   Collect_Coins_Task,
                    NULL,
                    60,
                    NULL,
                    SECOND_PRIORITY,
                    &gu8_collect_coins_handle
                );

    xTaskCreate (   Read_Cmd_Task,
                    NULL,
                    60,
                    NULL,
                    THIRD_PRIORITY,
                   &gu8_read_cmd_handle
                );

   xTaskCreate (   Give_Refund_Task,
                   NULL,
                   60,
                   NULL,
                   FOURTH_PRIORITY,
                   &gu8_give_refund
                );
     
    xTaskCreate (   Deliver_Coffee_Task,
                    NULL,
                    60,
                    NULL,
                    FIFTH_PRIORITY,
                    NULL
                ); 

    xTaskCreate (   Display_Messages_Task,
                    NULL,
                    100,
                    NULL,
                    FIFTH_PRIORITY,
                    &gu8_display_handle
                ); 
    vTaskStartScheduler();
    /* Error Happen Here. */
   
}

