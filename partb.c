#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define F_CPU 16000000UL

void Initialize()
{
	//disable interrupts
	cli();
	
	//prescale to 4
	CLKPR = (1 << CLKPCE);
	CLKPR = (1 << CLKPS1);
	
	//internal led output pin
	DDRB |= (1<<DDB5);
	
	
	//Timer 1 divide by 8
	TCCR1B |= (1<<CS11);
	
	//Timer 1 rising edge
	TCCR1B |= (1<<ICES1);
	
	
	//enable interrupt
	TIMSK1 |= (1<<ICIE1);
	
	sei();
}

ISR(TIMER1_CAPT_vect)
{
	PORTB ^= (1<<PORTB5);
	TCCR1B ^= (1<<ICES1);
}

int main(void)
{
	Initialize();
	while(1);
}
	
	