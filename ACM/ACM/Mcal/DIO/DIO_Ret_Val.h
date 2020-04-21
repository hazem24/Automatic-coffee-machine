/*
 * DIO_Ret_Val.h
 *
 * Created: 12/16/2019 7:34:04 PM
 *  Author: A.Elkady
 */ 


#ifndef DIO_RET_VAL_H_
#define DIO_RET_VAL_H_

/*
*	Error types of DIO module
*/

#define DIO_E_OK                         (Error)0

#define DIO_E_WRONG_PIN_NUMBER           (Error)1

#define DIO_E_WRONG_PORT_NUMBER          (Error)2

#define DIO_E_NEITHER_INPUT_NOR_OUTPUT   (Error)3

#define DIO_E_NEITHER_LOW_NOR_HIGH       (Error)4

#define DIO_E_PULL_UP_NOT_OK             (Error)5

#define DIO_E_NULL_POINTER               (Error)6

#endif /* DIO_RET_VAL_H_ */