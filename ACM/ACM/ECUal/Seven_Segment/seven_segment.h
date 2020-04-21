/*
 * Seven_Segment.h
 *
 * Created: 13-Apr-20 9:48:41 PM
 *  Author: LENOVO
 */ 


#ifndef SEVEN_SEGMENT_H_
#define SEVEN_SEGMENT_H_

/* Includes Section. */
#include "../../InfraStructure/std_types.h"

/* Seven Segment Port Number. */
#define SEVEN_SEGMENT_PORT_NUMBER 4


void Seven_Segment_Init();

void Seven_Segment_Write(uint8_t au8_port_value);

#endif /* SEVEN_SEGMENT_H_ */