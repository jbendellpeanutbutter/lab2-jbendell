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
volatile int phrase[] = {0,0,0,0,0};
volatile int place = 0;

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
		length = 0;
		edge = ICR1;
		
	} else {		
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
		
		
		if (pulse > 0 && pulse < 1875) {
		} else if (pulse > 1875 && pulse < 12500) 
		{
			sprintf(String,"DOT\n");
			UART_putstring(String);
			
			phrase[place] = 1;
			place = place + 1;
			
			PORTB |= (1 << PORTB1);
			_delay_ms(500);
			PORTB &= ~(1<<PORTB1);
			
		} else if (pulse > 12500 && pulse < 25000)
		{
			sprintf(String,"DASH\n");
			UART_putstring(String);
			
			phrase[place] = 2;
			place = place + 1;
			
			PORTB |= (1 << PORTB2);
			_delay_ms(500);
			PORTB &= ~(1<<PORTB2);
		} else 
		{
			sprintf(String,"SPACE\n");
			UART_putstring(String);
			
			phrase[place] = 3;
			decoder(phrase);
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

void decoder(int phrase[]) {
	char output = '!';
	if (phrase[0]==1 && phrase[1]==2 && phrase[2]==3 && phrase[3]==0 && phrase[4]==0) {
		output = 'A';
	} else if (phrase[0]==2 && phrase[1]==1 && phrase[2]==1 && phrase[3]==1 && phrase[4]==3) {
		output = 'B';
	} else if (phrase[0]==2 && phrase[1]==1 && phrase[2]==2 && phrase[3]==1 && phrase[4]==3) {
	output = 'C';
	}  else if (phrase[0]==2 && phrase[1]==1 && phrase[2]==1 && phrase[3]==3 && phrase[4]==0) {
	output = 'D';
	} else if (phrase[0]==1 && phrase[1]==3 && phrase[2]==0 && phrase[3]==0 && phrase[4]==0) {
	output = 'E';
	} else if (phrase[0]==1 && phrase[1]==1 && phrase[2]==2 && phrase[3]==1 && phrase[4]==3) {
	output = 'F';
	}  else if (phrase[0]==2 && phrase[1]==2 && phrase[2]==1 && phrase[3]==3 && phrase[4]==0) {
	output = 'G';
	}else if (phrase[0]==1 && phrase[1]==1 && phrase[2]==1 && phrase[3]==1 && phrase[4]==3) {
	output = 'H';
	} else if (phrase[0]==1 && phrase[1]==1 && phrase[2]==3 && phrase[3]==0 && phrase[4]==0) {
	output = 'I';
	}  else if (phrase[0]==1 && phrase[1]==2 && phrase[2]==2 && phrase[3]==2 && phrase[4]==3) {
	output = 'J';
	} else if (phrase[0]==2 && phrase[1]==1 && phrase[2]==2 && phrase[3]==3 && phrase[4]==0) {
	output = 'K';
	} else if (phrase[0]==1 && phrase[1]==2 && phrase[2]==1 && phrase[3]==1 && phrase[4]==3) {
	output = 'L';
	}  else if (phrase[0]==2 && phrase[1]==2 && phrase[2]==3 && phrase[3]==0 && phrase[4]==0) {
	output = 'M';
	} else if (phrase[0]==2 && phrase[1]==1 && phrase[2]==3 && phrase[3]==0 && phrase[4]==0) {
	output = 'N';
	} else if (phrase[0]==2 && phrase[1]==2 && phrase[2]==2 && phrase[3]==3 && phrase[4]==0) {
	output = 'O';
	}  else if (phrase[0]==1 && phrase[1]==2 && phrase[2]==2 && phrase[3]==1 && phrase[4]==3) {
	output = 'P';
	} else if (phrase[0]==2 && phrase[1]==2 && phrase[2]==1 && phrase[3]==2 && phrase[4]==3) {
	output = 'Q';
	} else if (phrase[0]==1 && phrase[1]==2 && phrase[2]==1 && phrase[3]==3 && phrase[4]==0) {
	output = 'R';
	}  else if (phrase[0]==1 && phrase[1]==1 && phrase[2]==1 && phrase[3]==3 && phrase[4]==0) {
	output = 'S';
	}else if (phrase[0]==2 && phrase[1]==3 && phrase[2]==0 && phrase[3]==0 && phrase[4]==0) {
	output = 'T';
	} else if (phrase[0]==1 && phrase[1]==1 && phrase[2]==2 && phrase[3]==3 && phrase[4]==0) {
	output = 'U';
	}  else if (phrase[0]==1 && phrase[1]==1 && phrase[2]==1 && phrase[3]==2 && phrase[4]==3) {
	output = 'V';
	} else if (phrase[0]==1 && phrase[1]==2 && phrase[2]==2 && phrase[3]==3 && phrase[4]==0) {
	output = 'W';
	} else if (phrase[0]==2 && phrase[1]==1 && phrase[2]==1 && phrase[3]==2 && phrase[4]==3) {
	output = 'X';
	}  else if (phrase[0]==2 && phrase[1]==1 && phrase[2]==2 && phrase[3]==2 && phrase[4]==3) {
	output = 'Y';
	}  else if (phrase[0]==2 && phrase[1]==2 && phrase[2]==1 && phrase[3]==1 && phrase[4]==3) {
	output = 'Z';
	}

	sprintf(String,"Decode value: %c\n\n", output);
	UART_putstring(String);
	
	place = 0;
	phrase[0] = 0;
	phrase[1] = 0;
	phrase[2] = 0;
	phrase[3] = 0;
	phrase[4] = 0;
}
				
	
int main(void)
{
	Initialize();
	
	while(1)
	{
	}
}




