/*
 * DIO_CONFIG.h
 *
 * Created: 11/1/2019 5:07:13 PM
 *  Author: A.Elkady
 */ 


#ifndef DIO_CONFIG_H_
#define DIO_CONFIG_H_

/*
*	Used macros
*/
#define BITS_PER_REGISTER 8

#define MAX_PORTS 4
#define MAX_PINS 32

#define INPUTS 0x00
#define OUTPUTS 0xFF

#define MAX 0xFF

/*
*	Definition of DIO ports
*/
#define PORT_A ( *((volatile unsigned char *) (0x3B) ) )
#define DDR_A  ( *((volatile unsigned char *) (0x3A) ) )
#define PIN_A  ( *((volatile unsigned char *) (0x39) ) )

#define PORT_B ( *((volatile unsigned char *) (0x38) ) )
#define DDR_B  ( *((volatile unsigned char *) (0x37) ) )
#define PIN_B  ( *((volatile unsigned char *) (0x36) ) )

#define PORT_C ( *((volatile unsigned char *) (0x35) ) )
#define DDR_C  ( *((volatile unsigned char *) (0x34) ) )
#define PIN_C  ( *((volatile unsigned char *) (0x33) ) )

#define PORT_D ( *((volatile unsigned char *) (0x32) ) )
#define DDR_D  ( *((volatile unsigned char *) (0x31) ) )
#define PIN_D  ( *((volatile unsigned char *) (0x30) ) )

/*
*	Definition of all DIO pins in a form of consecutive numbers from 0 : 31 (A0 : D7)
*/
#define A0 0
#define A1 1
#define A2 2
#define A3 3
#define A4 4
#define A5 5
#define A6 6
#define A7 7
#define B0 8
#define B1 9
#define B2 10
#define B3 11
#define B4 12
#define B5 13
#define B6 14
#define B7 15
#define C0 16
#define C1 17
#define C2 18
#define C3 19
#define C4 20
#define C5 21
#define C6 22
#define C7 23
#define D0 24
#define D1 25
#define D2 26
#define D3 27
#define D4 28
#define D5 29
#define D6 30
#define D7 31

#endif /* DIO_CONFIG_H_ */

