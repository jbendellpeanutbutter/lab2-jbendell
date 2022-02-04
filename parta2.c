#include <avr/io.h>
#include <util/delay.h>
#define F_CPU 16000000UL

void Initialize()
{
	//output LEDs
	DDRB |= (1 << DDB1);

	//input switch
	DDRD &= ~(1 << DDD7);
	
}

int main(void)
{
	Initialize();
	//turn all off
	PORTB &= ~(1<<PORTB1);

	
	while (1)
	{
		if (PIND & (1 << PIND7))
		{
			PORTB |= (1 << PORTB1);
		} else {
			PORTB &= ~(1<<PORTB1);
		}
		
	}
}