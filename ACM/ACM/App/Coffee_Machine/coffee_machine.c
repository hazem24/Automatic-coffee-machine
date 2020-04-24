/*
 * coffee_machine.c
 *
 * Created: 14-Apr-20 2:20:10 AM
 *  Author: LENOVO
 */ 
/*- INCLUDES ----------------------------------------------*/

#include "../../Service/FreeRtos/FreeRTOS.h"
#include "../../Service/FreeRtos/task.h"
#include "../../Service/FreeRtos/semphr.h"
#include "../../Service/FreeRtos/queue.h"
#include "../../Service/FreeRtos/timers.h"
#include "../../ECUal/Keypad/Keypad.h"
#include "../../ECUal/LCD/LCD.h"
#include "../../ECUal/Seven_Segment/seven_Segment.h"
#include "coffee_machine.h"
#include "coffee_machine_PBcfg.h"
#include <string.h>

/*- LOCAL MACROS ------------------------------------------*/

/**
 *	Number of steps needed by the system to init. 
 *  Steps is a number multiply of system tick. 
*/
#define INIT_STEPS   (uint8_t) 11


#define DISPLAY_TASK_MAILBOX_SIZE       (uint8_t)1

#define COFFEE_ID_MAILBOX_SIZE          (uint8_t)1

/* Time Needed To solve the Bouncing Problem. */
#define DEBOUNCING_DELAY_IN_MS         (uint8_t)50
/* Period Needed By LCD to Display character. */
#define CHARACTER_PERIOD_MS            (uint8_t)2

/* The Time For User To insert coins.  */
/* The Time for Coffee to deliver. */
#define COUNT_DOWN_PERIOD_MS           ((uint16_t)1000)


/* ID of the software timer used for display msg. */
#define SW_TIMER_DISPLAY_ID             (void*)0

/* ID of the software timer used for display msg. */
#define SW_TIMER_DISPLAY_ID_2           (void*)2

/* ID of count down Timer. */
#define SW_TIMER_COUNT_DOWN_TIMER_ID       (void*)1

/* Msg Id for Display Task. */
#define WELCOME_MSG_ID                      (uint8_t)1
#define MENU_MSG_ID                         (uint8_t)2
#define INSERT_COINS_MESSAGE_ID             (uint8_t)3
#define SORRY_MSG_ID                        (uint8_t)4
#define TAKE_REFUND_MSG_ID                  (uint8_t)5
#define PREPARING_COFFEE_ID                 (uint8_t)6
#define THANK_YOU_MSG_ID                    (uint8_t)7
/*- LOCAL Datatypes ----------------------------------------*/

/* Contents of Task_Display MailBox. */
typedef struct Task_Display
{
    uint8_t message_id;
    char message[LONGEST_MSG_LEN];
}Str_Display_Msg;

/* New Data Type Which Refer to Task Display Struct. */
typedef  Str_Display_Msg* ptr_to_task_display;


/* This Varaible User By the Tasks if it needs to send mail box to display Message Task. */
static Str_Display_Msg g_mail_box = { WELCOME_MSG_ID, WELCOME_MSG };

/* Pointer Point to MailBox of Display Task. */
static ptr_to_task_display g_ptr_display_mailbox = &g_mail_box;

/**
 * @brief Send Mailbox to display task with display msg.
 */
static void send_default_page();


/*- LOCAL FUNCTIONS PROTOTYPES ----------------------------*/

/**
 * @brief Display Task Call This Function when Message ID = 1 which means Welcome MSG.
 * @param a_ptr_products 
 */
static void COFFEE_MACHINE_Display_MainPage(const sint8_t au8_category_number,const Str_Products* a_ptr_products, const uint8_t au8_MSG_Type);
/**
 * @brief CallBack Function POST a semaphore which means LCD is Free.
 */
static uint8_t COFFEE_MACHINE_MSG_sent();

static void CONTROL_VALVE ( TimerHandle_t xTimer );

/**
 * @brief This is a callback function called 
 * when CHARACTER_PERIOD_MS passed so that send notification to LCD to display next Char.
 * @param xTimer 
 */
static void display_Char_CallBack( TimerHandle_t xTimer );


/**
 * @brief Check if input of User is Vailed Input.
 * @param au8_input 
 * @return uint8_t => TRUE.
 *                 => FALSE.
 */                 
static uint8_t is_vailed_input(const uint8_t au8_input);

/**
 * @brief Display Messages => LCD.
 * @note => All messages in the system displayed via this function except Main Page via "COFFEE_MACHINE_Display_MainPage".
 * @param au8_msg => msg to display. 
 */
static void display_msg(const char* au8_msg);


/**
 * @brief This is a callback function called when take coins time finished.
 * @param xTimer 
 */
static void take_coins_timer_CallBack( TimerHandle_t xTimer );

/*- GLOBAL STATIC VARIABLES -------------------------------*/

/** -- Objects Needed by Display Task. --**/
/* Create a mailbox which used to display messages via Display Task. */
static QueueHandle_t display_msg_mail_box = NULL;

/* Create Semaphore for display string call back. */
static SemaphoreHandle_t sema_display_string  = NULL; 

static SemaphoreHandle_t sema_display_char    = NULL;
/* This is a software timer which needed by display msg task. */
static TimerHandle_t display_characters_timer = NULL;

static SemaphoreHandle_t mutex_for_display_mailbox = NULL;
/** -- End Of Display Task Objects -- */

/* -- Objects for Collect Coins Task and Deliver Chocalate Task.*/

/* Handler for timer. */
static TimerHandle_t  g_count_down_timer = NULL;

/* Handler for timer2. */
static TimerHandle_t  g_count_down_timer2 = NULL;

/* semaphore notifying Collect Coins Task to start it's job. */
static SemaphoreHandle_t sema_allowable_time_for_insert_coins = NULL;

/* collect coins that user entered. */
static uint8_t gu8_collected_coins = FALSE;

/* mailbox keep the value of coffee id choosen by User.  */
static QueueHandle_t g_mailbox_coffee_id = FALSE;  
/* -- End Of Objects.  --*/

/*- GLOBAL EXTERN VARIABLES -------------------------------*/
 
/* Handler for some tasks. */

/* Handler for collect coins task. */
TaskHandle_t gu8_collect_coins_handle = NULL;
/* Handler for read cmd task. */
TaskHandle_t gu8_read_cmd_handle      = NULL;
/* Handler for display_task. */
TaskHandle_t gu8_display_handle       = NULL;
TaskHandle_t gu8_give_refund          = NULL;
/*-- End for handlers. --*/

/*-- Objects needed by Task Refund and deliver task.  --*/
static SemaphoreHandle_t sema_refund_task  = NULL; 
static SemaphoreHandle_t sema_deliver_coffee = NULL;
/*-- End of Objects needed by Task Refund  --*/

/* LOCAL FUNCTIONS IMPLEMENTATION ------------------------*/

/*- APIs IMPLEMENTATION -----------------------------------*/


/* Global Variable indicate if system is init or Not. */
static uint8_t gu8_is_init = FALSE;

static uint8_t gu8_cvalve_flag= FALSE; 

static uint8_t gu8_coffee_kind=NULL; 
/**
 * @brief Task to => Init all Modules needed.
 * @param a_ptr 
 */
void System_Init_Task(void* a_ptr)
{
    /* Create Semaphore to sync displaying chars. */
    /* Create Software Timer. */

	/* Init MailBox for Display_Msg_Task. */ 
	display_msg_mail_box = xQueueCreate( 
        DISPLAY_TASK_MAILBOX_SIZE , 
        sizeof(struct Task_Display*)
    );

    /* Init MailBox For coffee id. */
    g_mailbox_coffee_id = xQueueCreate
    (
        COFFEE_ID_MAILBOX_SIZE, 
        sizeof(uint8_t)
    );
    /* Init System. */
	Keypad_Init();
	Seven_Segment_Init();
	uint8_t au8_system_init_steps = FALSE;
	while( TRUE )
	{
		LCD_Init();
		au8_system_init_steps++;
		if ( au8_system_init_steps >= INIT_STEPS )
		{
			/* System init successfully. */
			gu8_is_init = TRUE;

            /* Create Binary SemaPhore For Synchorization with LCD. */
            sema_display_string = xSemaphoreCreateBinary();

            /* Give It By default. */
            xSemaphoreGive(sema_display_string);

            /* Create Binary SemaPhore to sync. chars inside LCD. */
            sema_display_char   = xSemaphoreCreateBinary();

            /* Create Binary Semaphore to sync. Deliver task. */
            sema_deliver_coffee = xSemaphoreCreateBinary();

            /* Create Binary SemaPhore to sync. refund task. */
            sema_refund_task    = xSemaphoreCreateBinary();
            /* Create Binary semaphore to sync. SW Timer with collect_coins_task. */
            sema_allowable_time_for_insert_coins = xSemaphoreCreateBinary();
            /* Create Mutex for mailbox => "Display Task." */
            mutex_for_display_mailbox = xSemaphoreCreateMutex();

            /* Create A software timer to track Chars Inside Specific String. */
            display_characters_timer = xTimerCreate
            (
               NULL, 
               pdMS_TO_TICKS(CHARACTER_PERIOD_MS), 
               pdTRUE, SW_TIMER_DISPLAY_ID, 
               display_Char_CallBack
            );
            
            /* Count Down Software Timer. */
            g_count_down_timer = xTimerCreate
            (
                NULL,
                COUNT_DOWN_PERIOD_MS,
                pdTRUE, SW_TIMER_COUNT_DOWN_TIMER_ID,
                take_coins_timer_CallBack
            );
			  /* Count Down Software Timer. */
		    g_count_down_timer2 = xTimerCreate
			    (
			       NULL,
			      2000,
			     pdTRUE, SW_TIMER_DISPLAY_ID_2 ,
			    CONTROL_VALVE
			    );
            /* Send MailBox for Task Display to Display Welcome Msg. */
            xQueueSend(display_msg_mail_box, &g_ptr_display_mailbox ,(TickType_t) portMAX_DELAY);

            /* active interrupt related to the coins. */
            INT_CFG_S a_str_interrupt = {INT_2, INT2_RISING_EDGE};
            INT_Init(&a_str_interrupt); 

            /* Activate Coins Interrupt. */
            /* Make Mutex of mailbox available By Default. */
            xSemaphoreGive(mutex_for_display_mailbox);
            /* Send this task to DORMANT State. */
			vTaskDelete( NULL );
		}
		vTaskDelay((CHARACTER_PERIOD_MS/portTICK_PERIOD_MS));
	}
}


/**
 * @brief This Task Responsable to collect coins from the user.
 * @priority 5 => First Priority in the system.
 * @param a_ptr 
 */
void Collect_Coins_Task( void* a_ptr )
{
    /* Check if the system is Init Successfully or not. */
    if ( FALSE == gu8_is_init )
    {
        /* Block System for the Init Time. */
        vTaskDelay(200/portTICK_PERIOD_MS);
    }
    else
    {
        /* Do Nothing. */
    }
    uint8_t sema_allowable_time_passed = NULL;
    uint8_t au8_take_mutex_mailbox = NULL, au8_coffee_id = FALSE;
    while ( TRUE )
    {
        if ( NULL != sema_allowable_time_passed )
        {
            /* collect coins logic. */
            /* check if the choosen product price. */
            
            /* Shared Resource With ISR. */
            /* It's more logic to disable source of interrupt only. */
            
            /* No coins Inserted. */
            taskENTER_CRITICAL();
                /* take value of collected coins. */
                uint8_t au8_collected_coins = gu8_collected_coins;
            taskEXIT_CRITICAL();
            
            if ( (uint8_t)au8_collected_coins <= (uint8_t)FALSE )
            {
                
                taskENTER_CRITICAL();
                    /* take value of collected coins. */
                    gu8_collected_coins = FALSE;
                taskEXIT_CRITICAL();
                
                au8_take_mutex_mailbox  = xSemaphoreTake( mutex_for_display_mailbox , (TickType_t)portMAX_DELAY);
                    /* Message To user that he/she did not put coins. */
                    g_mail_box.message_id   = SORRY_MSG_ID;
                    strcpy(  g_mail_box.message , SORRY_INSUFFICENT_COINS);
                    
                    xQueueSend(display_msg_mail_box, &g_ptr_display_mailbox, (TickType_t)0);
                
                    /* Wait for 5 Seconds. */
                    vTaskDelay((COUNT_DOWN_PERIOD_MS * (COUNT_DOWN_IN_SEC + 1))/portTICK_PERIOD_MS);
                    
                    /* Should Return Back To Idle State. */
                    /* Send another mailbox of idle state. */
                    send_default_page();
                    /* Naive Solution. */
                    /* TODO: Solve the problem Semaphore releasing twice by 
                    two condition inside switch case in Display Task. */
                    xSemaphoreTake(sema_allowable_time_for_insert_coins, (TickType_t)portMAX_DELAY);
                xSemaphoreGive( mutex_for_display_mailbox );
                /* Resume read cmd task. */
                vTaskResume( gu8_read_cmd_handle );
            }
            else
            {
                /* Coins inserted. */ 
                sema_allowable_time_passed = NULL; 
                xSemaphoreGive( sema_refund_task );
                /* Suspended this task. */
                /* This Task Resume by Deliver Coffee Task OR Refund_Task Based on 
                Specific logic inside Refund_task. */
                vTaskSuspend(NULL);
            }
            sema_allowable_time_passed = NULL; 
        }
        else
        {
            /* Check if allowable time was posted. */
            sema_allowable_time_passed = xSemaphoreTake(sema_allowable_time_for_insert_coins, (TickType_t)portMAX_DELAY);
        }
    }
}

/**
 * @brief This Task Responseable to take cmds from user via Keypad.
 * @priority 4 => Second Priority in the system.
 * @param a_ptr 
 */
void Read_Cmd_Task( void* a_ptr )
{

    /* Block This Task Until System init task finish. */
    if ( FALSE == gu8_is_init )
    {
        vTaskDelay(200/portTICK_PERIOD_MS);
    }
    else
    {
        /* NOP. */
    }
    /* When a cmd entered by user the app transfer to 

       * admin state   => Which owner entered magic CMD.
       * Collect Money => Normal User choosed specific coffee product.
     */
    uint8_t au8_key_pressed = FALSE ,au8_keypad_status = FALSE;
    uint8_t au8_row = FALSE, au8_col = FALSE, au8_stage = FALSE,
    au8_take_mutex_mailbox = NULL;

    /* Coffee_ID Choosen by User.  */
    uint8_t au8_coffee_id  = FALSE;

    while ( TRUE )
    {
        /* Check If user Pressed A Button. */
        Keypad_Get_Key(&au8_row, &au8_col);
        if (FALSE == au8_stage && NOKEY != au8_row && NOKEY != au8_col)
        {
            /* First Sample Taken. */
            au8_stage++;
        }
        else
        {
            /* Check if Second Sample of button the same as the first. */
            if ( NOKEY != au8_row && NOKEY != au8_col )
            {
                if ( NULL == au8_take_mutex_mailbox  )
                {
                    /* User Pressed A key. */
                    au8_key_pressed = gu8_keypad_buttons_interface[au8_row - 1][au8_col - 1];
                    
                    /* Check If input is vailed. */
                    if ( TRUE == is_vailed_input(au8_key_pressed - 0x30) )
                    {
                        /* Mutex for display task not Available try to take it.  */
                        au8_take_mutex_mailbox = xSemaphoreTake(mutex_for_display_mailbox, (TickType_t)portMAX_DELAY);

                        /* Mutex Available Write Your message Safely. */
                        au8_coffee_id = au8_key_pressed - 0x30;
						gu8_coffee_kind=au8_coffee_id; 
                        /* Insert Coins Page. */
                        g_mail_box.message_id = INSERT_COINS_MESSAGE_ID;
                        strcpy( g_mail_box.message, INSERT_COIN_MSG );
                        /* Reset Queue. */
                        xQueueReset(g_mailbox_coffee_id);
                        /* Send mailbox to Task => collect money with => gu8_coffee_id */
                        xQueueSend(g_mailbox_coffee_id, &au8_coffee_id, (TickType_t)portMAX_DELAY);
                        
                        xQueueSend(display_msg_mail_box, &g_ptr_display_mailbox, (TickType_t)portMAX_DELAY);
                        
                        /* Give Mutex Back. */
                        xSemaphoreGive(mutex_for_display_mailbox);
                        /* Suspend this task. */
                        vTaskSuspend(NULL);
                    }
                    else
                    {
                        /* Not A vailed Input behaviour.  */
                        /* TODO: May Be any Message for user indicate wrong input. */
                    }
                    au8_take_mutex_mailbox = NULL;
                }
                else
                {
                    /* NOP. */
                }
            }
            else
            {
                /* NOP. */
            }
            au8_stage = FALSE;
            au8_coffee_id = FALSE;            
        }
        /* Resolving Bounce Problem by take two sample of Key pressed each sample after 50 ms. */
        vTaskDelay(DEBOUNCING_DELAY_IN_MS/portTICK_PERIOD_MS);
    }
}

/**
 * @brief Give Refund For the user in two cases.
    * if the input coins < the price of the coffee.
    * if the user put coins > cofee price so the machine should return the differnce between it.
 * @priority 3 => Third Priority in the system.
 * @param a_ptr
 */
void Give_Refund_Task( void* a_ptr )
{
    /* Block This Task Until System init task finish. */
    if ( FALSE == gu8_is_init )
    {
        vTaskDelay(200/portTICK_PERIOD_MS);
    }
    else
    {
        /* NOP. */
    }
    /* Check if there's a coins inserted. */
    uint8_t sema_coins_inserted = NULL, 
    au8_coffee_id = NULL, au8_inserted_coins = NULL, 
    au8_take_mutex_mailbox = NULL;
    while( TRUE )
    {
        /* Read Coffee choosen id from mailbox. */

        /* check if user insert coins. */
        if ( NULL != sema_coins_inserted )
        {

            if ( au8_coffee_id != NULL )
            {
                taskENTER_CRITICAL();
                    au8_inserted_coins  = gu8_collected_coins;
                    gu8_collected_coins = FALSE; 
                taskEXIT_CRITICAL();

                /* Suspend Collect Coins. */
                vTaskSuspend( gu8_collect_coins_handle );
                /* Suspend read cmd task. */
                vTaskSuspend( gu8_read_cmd_handle );

                if ( au8_inserted_coins < gstr_products[au8_coffee_id - 1].coffee_price )
                {
                    /* if coins < product price */
                        /* Refund Coins. */
                        /* Message to user to take his/her refund. */
                        /* Transfer to IDLE State. */
                    Seven_Segment_Write( au8_inserted_coins );    
                    /* Mutex for display task not Available try to take it.  */
                    au8_take_mutex_mailbox = xSemaphoreTake(mutex_for_display_mailbox, (TickType_t)portMAX_DELAY); 
                    
                        g_mail_box.message_id =  TAKE_REFUND_MSG_ID;
                        strcpy(g_mail_box.message, TAKE_YOUR_COINS);
                    
                        xQueueSend(display_msg_mail_box, &g_ptr_display_mailbox , (TickType_t)portMAX_DELAY);
                        /* Naive Solution. */
                        
                        /* Block This Task. */
                        vTaskDelay((COUNT_DOWN_PERIOD_MS * (COUNT_DOWN_IN_SEC + 1))/portTICK_PERIOD_MS); 

                        send_default_page();
                        /* TODO: Solve the problem Semaphore releasing twice by 
                        two condition inside switch case in Display Task. */
                        xSemaphoreTake(sema_allowable_time_for_insert_coins, (TickType_t)portMAX_DELAY);

                    xSemaphoreGive(mutex_for_display_mailbox);

                    /* Resume Collect Coins. */
                    vTaskResume( gu8_collect_coins_handle );
                    /* Resume read cmd task. */
                    vTaskResume( gu8_read_cmd_handle );
                }
                else
                {
                    if (( au8_inserted_coins > gstr_products[au8_coffee_id - 1].coffee_price ))
                    {
                        /* Send Mailbox for Display Task to inform user that coffee is preparing. */
                            /* if coins > product price */
                            /* give refund. */
                            /* transfer to Deliver coffee task. */
                     
                        au8_take_mutex_mailbox = xSemaphoreTake(mutex_for_display_mailbox, (TickType_t)portMAX_DELAY);
                            
                            Seven_Segment_Write( au8_inserted_coins - gstr_products[au8_coffee_id - 1].coffee_price );
                            
                            g_mail_box.message_id =  TAKE_REFUND_MSG_ID;
                            strcpy(g_mail_box.message, TAKE_YOUR_COINS);
                            
                            xQueueSend(display_msg_mail_box, &g_ptr_display_mailbox , (TickType_t)portMAX_DELAY);
                            /* Block This Task. */
                            vTaskDelay((COUNT_DOWN_PERIOD_MS * COUNT_DOWN_IN_SEC)/portTICK_PERIOD_MS); 

                            /* Sync. with Deliver coffee task. */
                            xSemaphoreGive( sema_deliver_coffee );
                        xSemaphoreGive(mutex_for_display_mailbox);
                    }
                    else
                    {
                        /**
                         * Coins == price.
                         * Sync. with semaphore with Deliver coffee task.
                         */
                        xSemaphoreGive( sema_deliver_coffee );
                    }
                    /* reset coins (7 Segment.) */
                    Seven_Segment_Write( FALSE );
                }
            }            
            sema_coins_inserted = NULL;
        }
        else
        {
            au8_coffee_id = NULL;
            /* Waiting notication from collect coins. */
            sema_coins_inserted = xSemaphoreTake(sema_refund_task, (TickType_t)portMAX_DELAY);
            xQueueReceive( g_mailbox_coffee_id, &au8_coffee_id, (TickType_t)portMAX_DELAY );
        }
    }
}

/**
 * @brief Deliver The Choosen Coffee For the user.
 * @priority 2 => Fourth Priority in the system.
 * @param a_ptr 
 */
void Deliver_Coffee_Task( void* a_ptr )
{
    /* Block This Task Until System init task finish. */
    if ( FALSE == gu8_is_init )
    {
        vTaskDelay(200/portTICK_PERIOD_MS);
    }
    else
    {
        /* NOP. */
    }

    uint8_t deliver_coffee = NULL;
    uint8_t au8_take_mutex_mailbox = NULL;

    while( TRUE )
    {
        /* check if a notification signal comes correctly from refund task. */
        if ( NULL != deliver_coffee )
        {
            Seven_Segment_Write( FALSE );

             /* Suspend Collect Coins. */
            vTaskSuspend( gu8_collect_coins_handle );
            /* Suspend read cmd task. */
            vTaskSuspend( gu8_read_cmd_handle );
            /* Suspend Collect coins. */
            vTaskSuspend( gu8_give_refund );
            au8_take_mutex_mailbox = xSemaphoreTake( mutex_for_display_mailbox  , (TickType_t)portMAX_DELAY);
            
            /* Send A message to user .. preparing coffee. */
            
            /* TODO: mechanical System Should works here. */
            
            /* Transfer this task to waiting state. */
            vTaskDelay(( COUNT_DOWN_IN_SEC * COUNT_DOWN_PERIOD_MS)/portTICK_PERIOD_MS); 
    
            g_mail_box.message_id = PREPARING_COFFEE_ID;

            strcpy( g_mail_box.message, "Preparing" );
            
            xQueueSend( display_msg_mail_box, &g_ptr_display_mailbox, (TickType_t)0 );
			
			xTimerStart(g_count_down_timer2, (TickType_t)portMAX_DELAY);
			
            /* Transfer this task to waiting state. */
            vTaskDelay(( COUNT_DOWN_IN_SEC * COUNT_DOWN_PERIOD_MS)/portTICK_PERIOD_MS); 

            /* Send Thank you Message. */
            g_mail_box.message_id = THANK_YOU_MSG_ID;

            strcpy( g_mail_box.message, "Thanks" );

            xQueueSend( display_msg_mail_box, &g_ptr_display_mailbox, (TickType_t)0 );

            /* Transfer this task to waiting state. */
            vTaskDelay(( COUNT_DOWN_IN_SEC * COUNT_DOWN_PERIOD_MS)/portTICK_PERIOD_MS); 
            /* Naive Solution. */

            xSemaphoreGive( mutex_for_display_mailbox );


            send_default_page();
            
            /* TODO: Solve the problem Semaphore releasing twice by 
            two condition inside switch case in Display Task. */
            xSemaphoreTake(sema_allowable_time_for_insert_coins, (TickType_t)portMAX_DELAY);

            /* Resume Collect Coins. */
            vTaskResume( gu8_collect_coins_handle );
            /* Resume read cmd task. */
            vTaskResume( gu8_read_cmd_handle );

            vTaskResume( gu8_give_refund );
            /* reset the flag. */
            deliver_coffee = NULL;
        }
        else
        {
            /* Deliver Coffee ? */
            deliver_coffee = xSemaphoreTake( sema_deliver_coffee , (TickType_t)portMAX_DELAY);
        }
    }
}
/** @brief This Task Responsiable to display messages to user.
 * @priority 1 => Lowest Priority in the system.
 * @param au8_ptr 
 */
void Display_Messages_Task( void* a_ptr )
{
	/* Hold Number of Chars to be Display. */
	uint8_t au8_char_to_display = FALSE;
	uint8_t au8_counter         = FALSE;
	uint8_t au8_locker          = FALSE;

    /* Check if the system is Init Successfully or not. */
    if ( FALSE == gu8_is_init )
    {
        /* Block System for the Init Time. */
       vTaskDelay(200/portTICK_PERIOD_MS);
    }
    else
    {
		/*NOP.*/
    }

    /**
    * 1 - In Idle State This Task output => "Welcome Msg" + Menu.
    * 2 - In progress state => user insert coins => 30 sec down.
    * 3 - If In step '2' user insert sufficent amount of coins => give  
    */
    /* Buffer for MailBox. */
    ptr_to_task_display mailbox;
    uint8_t au8_take_mutex_mailbox = NULL;
    while( TRUE )
    {
        /* If it taken so another task is writting on it in this instance. */
        if ( (TRUE == au8_locker) || pdPASS  == xQueueReceive( display_msg_mail_box, &mailbox, portMAX_DELAY ) )
        {
            /* Check the length of the incoming msg. */
            if ( FALSE == au8_locker ) /* Read Length only if locker is false which means new message came. */
            {
                au8_locker  = TRUE;
            }
            else
            {       
                /* Clear LCD First. */
                LCD_Clear_Display();
                
                /* Display The Message. */
                /* Start SW Timer. */
                xTimerStart(display_characters_timer, (TickType_t)portMAX_DELAY);
               
                /* First Indicate any message is coming by message ID. */
                switch ( mailbox->message_id )
                {
                    case WELCOME_MSG_ID:
                    /* I need to print Menu. */        
                    /* I added The Condition to NUMBER_OF_PRODUCTS + 1 to add welcome Message. */
                    for(sint8_t i = -1; i < NUMBER_OF_PRODUCTS; ++i )
                    {
                        /* Wait Notification Signal From LCD. */
                        xSemaphoreTake(sema_display_string, (TickType_t) portMAX_DELAY );

                        if ( (-1) == i )
                        {
                            COFFEE_MACHINE_Display_MainPage(i, &gstr_products, WELCOME_MSG_ID);
                        }
                        else
                        {
                            COFFEE_MACHINE_Display_MainPage(i, &gstr_products, MENU_MSG_ID);
                        }
                    }      
                    break;
                    case INSERT_COINS_MESSAGE_ID:
                    case SORRY_MSG_ID:
                    case TAKE_REFUND_MSG_ID:
                        /* Display Insert Coins Message + Timer. */
                        strcat( mailbox->message, TIMER_MSG );
                        display_msg( mailbox->message  );
                    break;
                    case PREPARING_COFFEE_ID:
                    case THANK_YOU_MSG_ID:
                        display_msg( mailbox->message );
                    default:
                    break;
                }
                /* Stop SW Timer. */
                xTimerStop(display_characters_timer, (TickType_t)0);

                /* Start SW Timer for counter down. */
                if ( mailbox->message_id != WELCOME_MSG_ID && mailbox->message_id != THANK_YOU_MSG_ID) xTimerStart(g_count_down_timer, (TickType_t)0);
                
                /* Wait for new Message to Come. */
                au8_locker          = FALSE;
                au8_counter         = FALSE;
                au8_char_to_display = FALSE;
            }
        }
        else
        {
            /* Do Nothing. */
        }
    }
} 

/**
 * @brief Display Messages => LCD.
 * @note => All messages in the system displayed via this function except Main Page via "COFFEE_MACHINE_Display_MainPage".
 * @param au8_msg => msg to display. 
 */
static void display_msg(const char* au8_msg)
{
    /* Check For NULL pointer. */
    if ( NULL != au8_msg )
    {
        uint8_t au8_len = strlen(au8_msg);
        for(uint8_t i = 0; i <= au8_len; ++i)
        {
            LCD_Display_String(au8_msg, au8_len, COFFEE_MACHINE_MSG_sent);
            /* Wait Display Character Semaphore to release so that this means LCD Ready can display Another Char. */
            xSemaphoreTake(sema_display_char, portMAX_DELAY);
        }
    }
    else
    {
        /* NOP. */
    }
}
/**
 * @brief Display Task Call This Function when Message ID = 1 which means Welcome MSG.
 * @param a_ptr_products 
 */
static void COFFEE_MACHINE_Display_MainPage(const sint8_t au8_category_number,const Str_Products* a_ptr_products, const uint8_t au8_MSG_Type)
{
    static uint8_t u8_mainPage[LONGEST_MSG_LEN];
    sint8_t au8_row = au8_category_number + 1, 
    au8_col = FIRST_COL, au8_price[3] = {0}, au8_number[2] = {0};
    

    /* Choose The Right ROW. */
    LCD_Goto(au8_row, au8_col);

    /* Display product id. */
    /* Display price. */
    /* Display Product Name. */ 
    u8_mainPage[0] = '\0';
    switch (au8_MSG_Type)
    {
    case WELCOME_MSG_ID:
        strcat(u8_mainPage, WELCOME_MSG);
        break;
    case MENU_MSG_ID:
        /* Display Main Menu. */

        /* Add The Currency Type. */
        au8_price[1]   = CURRENCY_TYPE;
        /* Convert Number and Price form character to string. */
        au8_price[0]   = a_ptr_products[au8_category_number].coffee_price + 0x30;
        au8_number[0]  = a_ptr_products[au8_category_number].coffee_id + 0x30;

        /* Number of Product. */
        strcat(u8_mainPage, au8_number);
        /* Name of product. */
        strcat(u8_mainPage, a_ptr_products[au8_category_number].coffee_type);
        /* Price of product. */
        strcat(u8_mainPage, au8_price);
        break;
    default:
        break;
    } 

    uint8_t au8_len = strlen(u8_mainPage);
    /* send this string to strcpy. */
    
    for(uint8_t i = 0; i <= au8_len; ++i)
    {
        LCD_Display_String(u8_mainPage, au8_len, COFFEE_MACHINE_MSG_sent);
        /* Wait Display Character Semaphore to release so that this means LCD Ready can display Another Char. */
        xSemaphoreTake(sema_display_char, portMAX_DELAY);
    }
}


/**
 * @brief This is a callback function called 
 * when CHARACTER_PERIOD_MS passed so that send notification to LCD to display next Char.
 * @param xTimer 
 */
static void display_Char_CallBack( TimerHandle_t xTimer )
{
    /* Give Semaphore of Display Chars. */
    xSemaphoreGive( sema_display_char );
}
static void CONTROL_VALVE ( TimerHandle_t xTimer )
{
	PORTD=0XF0; 
	if( 1==gu8_coffee_kind)
	{
		DDRB |=(1<<7); 
		PORTB|=(1<<7); 
		PORTB&=~(1<<6);
		PORTB&=~(1<<5);
		gu8_cvalve_flag=0;
		xTimerStop( g_count_down_timer2, (TickType_t)portMAX_DELAY );
	}
	else if (2==gu8_coffee_kind)
	{
		if(gu8_cvalve_flag==0)
		{
			DDRB |=(1<<7); 
			DDRB |=(1<<6); 
			DDRB |=(1<<5); 
		   PORTB|=(1<<6); 
		   PORTB&=~(1<<7);
		   PORTB&=~(1<<5);
		   gu8_cvalve_flag++; 
		}
		else if(gu8_cvalve_flag==1)
		{
			gu8_cvalve_flag++;
		}
		else if(gu8_cvalve_flag==2)
		{
			PORTB|=(1<<7);
			PORTB&=~(1<<6);
			PORTB&=~(1<<5);
			gu8_cvalve_flag++;
		}
		else if(gu8_cvalve_flag==3)
		{
			PORTB&=~(1<<6);
			PORTB&=~(1<<7);
			PORTB&=~(1<<5);
			gu8_cvalve_flag=0;
			xTimerStop( g_count_down_timer2, (TickType_t)portMAX_DELAY );
		}
	}
	else if(3==gu8_coffee_kind)
	{
		if(gu8_cvalve_flag==0)
		{
			DDRB |=(1<<7); 
			DDRB |=(1<<6); 
			DDRB |=(1<<5); 
		   PORTB|=(1<<5); 
		   PORTB&=~(1<<7);
		   PORTB&=~(1<<6);
		   gu8_cvalve_flag++; 
		}
		else if(gu8_cvalve_flag==1)
		{
			gu8_cvalve_flag++;
		}
		else if(gu8_cvalve_flag==2)
		{
			PORTB|=(1<<7);
			PORTB&=~(1<<6);
			PORTB&=~(1<<5);
			gu8_cvalve_flag++;
		}
		else if(gu8_cvalve_flag==3)
		{
			PORTB&=~(1<<6);
			PORTB&=~(1<<7);
			PORTB&=~(1<<5);
			gu8_cvalve_flag=0;
			xTimerStop( g_count_down_timer2, (TickType_t)portMAX_DELAY );
		}
	}
	else
	{
			
	}
}
/**
 * @brief This is a callback function called when take coins time finished.
 * @param xTimer 
 */
static void take_coins_timer_CallBack( TimerHandle_t xTimer )
{
    /* Give A semaphore to Collect_Coins Task.  */
    static sint8_t au8_counter = COUNT_DOWN_IN_SEC, i = 0;

    au8_counter--;
    
    if ( (sint8_t)au8_counter < (sint8_t)FALSE )
    {
        au8_counter = COUNT_DOWN_IN_SEC;
        xTimerStop( g_count_down_timer, (TickType_t)portMAX_DELAY );
        /* Send Semaphore to Collect Coins Task to start it's logic. */
        xSemaphoreGive(sema_allowable_time_for_insert_coins);
    }
    else
    {
        if ( (sint8_t)FALSE != (sint8_t)au8_counter )
        {
            LCD_Goto(THIRD_ROW, 18);
            LCD_Display_Char((au8_counter / 10) + 0x30);
            LCD_Display_Char((au8_counter % 10) + 0x30);
        }
    }
}
/**
 * @brief CallBack Function POST a semaphore which means LCD is Free.
 */
static uint8_t COFFEE_MACHINE_MSG_sent()
{
    /* Send Notification to Display TASK. */
    xSemaphoreGive(sema_display_string);
    return TRUE;
}

/**
 * @brief Check if input of User is Vailed Input.
 * @param au8_input 
 * @return uint8_t => TRUE.
 *                 => FALSE.
 */                 
static uint8_t is_vailed_input(const uint8_t au8_input)
{
    uint8_t au8_is_valied = FALSE;
    /* Check if it's a product input. */
    if ( NUMBER_OF_PRODUCTS >= (au8_input) )
    {
        /* It means Normal User Choose Right Coffee. */
        au8_is_valied = TRUE;
    }
    else
    {
        /* Check For Owner Cmd. */
    }
    return au8_is_valied;
}


/**
 * @brief Send Mailbox to display task with display msg.
 */
static void send_default_page()
{
    /* Send To Display Task => Default Page (Coffee Prices). */
    /* Access Mailbox safely. */
    g_mail_box.message_id = WELCOME_MSG_ID;

    strcpy( g_mail_box.message, WELCOME_MSG );

    xQueueReset(display_msg_mail_box);

    /* Send A new mailbox for Display Task. */
    xQueueSend(display_msg_mail_box, &g_ptr_display_mailbox, (TickType_t)0);

}
ISR(INT2_vect)
{
    gu8_collected_coins++;
}