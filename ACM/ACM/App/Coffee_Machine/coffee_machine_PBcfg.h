/*
 * coffee_machine_PBcfg.h
 *
 * Created: 14-Apr-20 3:14:57 AM
 *  Author: LENOVO
 */ 


#ifndef COFFEE_MACHINE_PBCFG_H_
#define COFFEE_MACHINE_PBCFG_H_


/*- INCLUDES -----------------------------------------------*/
#include "../../InfraStructure/std_types.h"
#include "../../Mcal/INT/int.h"

/*- CONSTANTS ----------------------------------------------*/

#define LONGEST_MSG_LEN    (uint8_t) 60


#define NUMBER_OF_PRODUCTS  (uint8_t)3

/* PRODUCTS (Options). */
#define WELCOME_MSG             " ** illy Coffee ** "
#define INSERT_COIN_MSG         "Insert Coins "
#define TIMER_MSG               " wait: "
#define PLEASE_WAIT_MSG         "Please Wait : "
#define SORRY_INSUFFICENT_COINS "SRY Insufficent coins"
#define PREPARING_COFFEE        "Preparing"
#define TAKE_YOUR_COINS         "Refund"
#define THANK_YOU               "Thanks"
#define CAPPUCCINO_MSG                    " Cappuccino  "
#define NORMAL_COFFEE_OPTION_MSG          " Normal Coffee "
#define COFFEE_WITH_MILK_OPTION_MSG       " Milk Coffee "

/* 
    * Counter Down in sec.
    * This Time needed for coffee to deliver for the user.
    * This Time also used as allowable time for user to enter 
*/
#define COUNT_DOWN_IN_SEC              ((uint8_t)5)


/* Pin at which Coins Inserted. */
#define COINS_PIN                    PB2
/**
 * @brief Price => products.
 * May Be in => '$'
 *              '€'
 *              '£'
 * @note => Should Be One Character only to work Probable.
 */
#define CURRENCY_TYPE      (sint8_t)'$'
/*- PRIMITIVE TYPES ----------------------------------------*/
/*- ENUMS --------------------------------------------------*/

/* Ids of the products. */
typedef enum 
{
    NORMAL_COFFEE = 1,
    COFFEE_WITH_MILK = 2,
    CAPPUCCINO = 3
}Enum_Coffee_Ids;


/* Prices of the products In '$'. */
typedef enum
{
    NORMAL_COFFEE_PRICE    = 2,
    COFFEE_WITH_MILK_PRICE = 4,
    CAPPUCCINO_PRICE       = 6
}Enum_Coffee_Prices;


/*- STRUCTS AND UNIONS -------------------------------------*/
typedef struct products
{
    uint8_t coffee_id; /* id of the coffee. */
    uint8_t coffee_price;
    char*   coffee_type;
}Str_Products;
/*- FUNCTION-LIKE MACROS -----------------------------------*/

extern const Str_Products gstr_products[NUMBER_OF_PRODUCTS];

#endif /* COFFEE_MACHINE_PBCFG_H_ */