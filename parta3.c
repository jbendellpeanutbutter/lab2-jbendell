#include <avr/io.h>
#include <util/delay.h>
#define F_CPU 16000000UL

void Initialize()
{
	//output LEDs
	DDRB |= (1 << DDB1);
	DDRB |= (1 << DDB2);
	DDRB |= (1 << DDB3);
	DDRB |= (1 << DDB4);

	//input switch
	DDRD &= ~(1 << DDD7);
	
}

int main(void)
{
	Initialize();
	//turn all off
	PORTB &= ~(1<<PORTB1);
	PORTB &= ~(1<<PORTB2);
	PORTB &= ~(1<<PORTB3);
	PORTB &= ~(1<<PORTB4);
	
	int light = 1;
	
	while (1)
	{
		if (light == 1) {
			PORTB |= (1 << PORTB1);
			//turn the rest off
			PORTB &= ~(1<<PORTB2);
			PORTB &= ~(1<<PORTB3);
			PORTB &= ~(1<<PORTB4);
		} else if (light == 2) {
			PORTB |= (1<<PORTB2);
			//turn the rest off
			PORTB &= ~(1<<PORTB1);
			PORTB &= ~(1<<PORTB3);
			PORTB &= ~(1<<PORTB4);
		} else if (light == 3) {
			PORTB |= (1<<PORTB3);
			//turn the rest off
			PORTB &= ~(1<<PORTB2);
			PORTB &= ~(1<<PORTB1);
			PORTB &= ~(1<<PORTB4);
		} else {
			PORTB |= (1<<PORTB4);
			//turn the rest off
			PORTB &= ~(1<<PORTB2);
			PORTB &= ~(1<<PORTB3);
			PORTB &= ~(1<<PORTB1);
		}

		if (PIND & (1 << PIND7))
		{
			light = light + 1;
			if (light == 5) {
				light = 1;
				}
			_delay_ms(10000);
		}
		
	}
}