#ifndef REGISTERS_H_
#define REGISTERS_H_
/*GPIO PORTS ADDRESSES */
#define PORTA (*(volatile uint8_t *)0x003B)
#define PORTB (*(volatile uint8_t *)0x0038)
#define PORTC (*(volatile uint8_t *)0x0035)
#define PORTD (*(volatile uint8_t *)0x0032)

#define PINA (*(volatile uint8_t *)0x0039)
#define PINB (*(volatile uint8_t *)0x0036)
#define PINC (*(volatile uint8_t *)0x0033)
#define PIND (*(volatile uint8_t *)0x0030)

#define DDRA (*(volatile uint8_t *)0x003A)
#define DDRB (*(volatile uint8_t *)0x0037)
#define DDRC (*(volatile uint8_t *)0x0034)
#define DDRD (*(volatile uint8_t *)0x0031)

/*Control Register*/
#define SREG (*(volatile uint8_t *)0x005F)

/* SREG Register Pins*/
#define C  0
#define Z  1
#define N  2
#define V  3
#define S  4
#define H  5
#define T  6
#define I  7

/*INTERRUPT REGISTERS ADDRESSES*/
#define GICR (*(volatile uint8_t *) 0x005B)
#define GIFR (*(volatile uint8_t *) 0x005A)
#define MCUCR (*(volatile uint8_t *)0x0055)
#define MCUCSR (*(volatile uint8_t *)0x0054)

/* GICR Register Pins*/
#define IVCE  0
#define IVSEL 1
#define INT2  5
#define INT0  6
#define INT1  7

/* GIFR Register Pins*/
#define INTF2 5
#define INTF0 6
#define INTF1 7

/*MCUCR Register Pins*/
#define ISC00  0
#define ISC01  1
#define ISC10  2
#define ISC11  3
#define SM0    4
#define SM1    5
#define SM2    6
#define SE     7

/*MCUCSR Register Pins*/
#define PORF  0
#define EXTRF 1
#define BORF  2
#define WDRF  3
#define JTRF  4
#define ISC2  6
#define JTD   7

/*TIMER REGISTERS ADDRESSES*/
#define TCCR0 (*(volatile uint8_t *)0x0053)
#define TCNT0 (*(volatile uint8_t *)0x0052)
#define OCR0 (*(volatile uint8_t *)0x005C)
#define TIMSK (*(volatile uint8_t *)0x0059)
#define TIFR (*(volatile uint8_t *)0x0058)

#define TCCR1A (*(volatile uint8_t *)0x004F)
#define TCCR1B (*(volatile uint8_t *)0x004E)
#define TCNT1H (*(volatile uint8_t *)0x004D)
#define TCNT1L (*(volatile uint8_t *)0x004C)
#define OCR1AH (*(volatile uint8_t *)0x004B)
#define OCR1AL (*(volatile uint8_t *)0x004A)
#define OCR1BH (*(volatile uint8_t *)0x0049)
#define OCR1BL (*(volatile uint8_t *)0x0048)
#define ICR1H (*(volatile uint8_t *)0x0047)
#define ICR1L (*(volatile uint8_t *)0x0046)

#define TCCR2 (*(volatile uint8_t *)0x0045)
#define TCNT2 (*(volatile uint8_t *)0x0044)
#define OCR2 (*(volatile uint8_t *)0x0043)
/*TIMSK Register pins*/
#define TOIE0  0
#define OCIE0  1
#define TOIE1  2
#define OCIE1B 3
#define OCIE1A 4
#define TICIE1 5
#define TOIE2  6
#define OCIE2  7

/*TIFR Register Timer0 pins*/
#define TOV0  0
#define OCF0  1
#define TOV1  2
#define OCF1B 3
#define OCF1A 4
#define ICF1  5
#define TOV2  6
#define OCF2  7

/* TCCR0 register pins*/
#define CS00  0
#define CS01  1
#define CS02  2
#define WGM01 3
#define COM00 4
#define COM01 5
#define WGM00 6
#define FOC0  7

/* TCCR1A register pins*/
#define WGM10  0
#define WGM11  1
#define FOC1B  2
#define FOC1A  3
#define COM1B0 4
#define COM1B1 5
#define COM1A0 6
#define COM1A1 7

/* TCCR1B register pins*/
#define CS10  0
#define CS11  1
#define CS12  2
#define WGM12 3
#define WGM13 4
#define ICES1 6
#define ICNC1 7

/* TCCR2 register pins*/
#define CS20  0
#define CS21  1
#define CS22  2
#define WGM21 3
#define COM20 4
#define COM21 5
#define WGM20 6
#define FOC2 7

#define SFIOR (*(volatile uint8_t *)0x0050)
#define ADMUX (*(volatile uint8_t *)0x0027)
#define ADCSRA (*(volatile uint8_t *)0x0026)
#define ADCH (*(volatile uint8_t *)0x0025)
#define ADCL (*(volatile uint8_t *)0x0024)


/*SFIOR register Pins*/
#define PSR10  0
#define PSR2   1
#define PUD    2
#define ACME   3
#define ADTS0  5
#define ADTS1  6
#define ADTS2  7


/*ADMUX register Pins*/
#define MUX0  0
#define MUX1  1
#define MUX2  2
#define MUX3  3
#define MUX4  4
#define ADLAR 5
#define REFS0 6
#define REFS1 7


/*ADCSRA register Pins*/
#define ADPS0  0
#define ADPS1  1
#define ADPS2  2
#define ADIE   3
#define ADIF   4
#define ADATE  5
#define ADSC   6
#define ADEN   7
#endif /* REGISTERS_H_ */
