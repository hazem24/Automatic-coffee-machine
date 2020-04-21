/*
 * coffee_machine_cfg.c
 *
 * Created: 14-Apr-20 3:03:34 AM
 *  Author: LENOVO
 */ 

#include "coffee_machine_PBcfg.h"

const Str_Products gstr_products[NUMBER_OF_PRODUCTS] = 
{
    { NORMAL_COFFEE, NORMAL_COFFEE_PRICE, NORMAL_COFFEE_OPTION_MSG },
    { COFFEE_WITH_MILK, COFFEE_WITH_MILK_PRICE, COFFEE_WITH_MILK_OPTION_MSG },
    { CAPPUCCINO, CAPPUCCINO_PRICE, CAPPUCCINO_MSG }
};