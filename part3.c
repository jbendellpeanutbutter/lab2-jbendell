#define F_CPU 16000000UL
#define BAUD_RATE 9600
#define BAUD_PRESCALER (((F_CPU / (BAUD_RATE * 16UL))) - 1)

#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "uart.h"

char String[25];

volatile int edge = 0;
volatile int length = 0;
volatile int pulse = 0;

void Initialize()
{
	//disable interrupts
	cli();
	
	//serial set up
	UART_init(BAUD_PRESCALER);
	sprintf(String,"Morse Code Time! \n");
	UART_putstring(String);
	
	//output LEDs
	DDRB |= (1 << DDB1);
	DDRB |= (1 << DDB2);
	
	//Timer 1 divide by 256
	TCCR1B |= (1<<CS12);
	
	//enable interrupt
	TIMSK1 |= (1<<ICIE1);
	
	//enable overflow interrupt
	TIMSK1 |= (1<<TOIE1);
	
	//look for rising edge
	TCCR1B |= (1<<ICES1);
	
	//noise canceler
	TCCR1B |= (1<<ICNC1);
	
	//clear flags
	TIFR1 |= (1<<ICF1);
	TIFR1 |= (1<<TOV1);
	
	sei();
}

ISR(TIMER1_CAPT_vect)
{
	if (TCCR1B & (1<<ICES1))
	{
		sprintf(String,"Value1 value: %u\n", ICR1);
		UART_putstring(String);
		length = 0;
		edge = ICR1;
		
	} else {
		sprintf(String,"Value2 value: %u\n", ICR1);
		UART_putstring(String);
		
		sprintf(String,"Length value: %u\n", length);
		UART_putstring(String);
		
		int dur = 65535 - edge;
		if (length > 1) {
			pulse = 65000;
		} else if (length == 0) {
			pulse = ICR1 - edge;
		} else if (length == 1 && dur < 25000 && ICR1 < 25000) {
			pulse = 65530 - edge + ICR1;
		} else if (length == 1 && (dur > 25000 || ICR1 > 25000)) {
			pulse = 65000;
		}
		
		
		sprintf(String,"Pulse value: %u\n", pulse);
		UART_putstring(String);
		
		if (pulse > 0 && pulse < 1875) {
			sprintf(String,"nutting\n\n");
			UART_putstring(String);
		} else if (pulse > 1875 && pulse < 12500) 
		{
			sprintf(String,"DOT\n\n");
			UART_putstring(String);
		} else if (pulse > 12500 && pulse < 25000)
		{
			sprintf(String,"DASH\n\n");
			UART_putstring(String);
		} else 
		{
			sprintf(String,"SPACE\n\n");
			UART_putstring(String);
		}
		
		_delay_ms(100);
	}
	
	
	//toggle falling and rising edge
	TCCR1B ^= (1<<ICES1);
	
	//clear flags
	TIFR1 |= (1<<ICF1);
	TIFR1 |= (1<<TOV1);
}

ISR(TIMER1_OVF_vect) {
	length = length + 1;
}

int main(void)
{
	Initialize();
	
	while(1)
	{
	}
}




