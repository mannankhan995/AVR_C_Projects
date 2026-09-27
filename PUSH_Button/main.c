#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#define LED_PIN			PORTB5
#define PUSH_BUTTON		PORTB0


int main(void)
{
	DDRB |= (1 << LED_PIN);							// Setting the pin 13 as output
	DDRB &= ~(1 << PUSH_BUTTON);					// Setting the pin 8 as Input
	PORTB |= (1 << PUSH_BUTTON);					// pull up
	uint8_t Last_State = 1;
	uint8_t Current_State;
    while (1) 
    {
		Current_State = (PINB & (1 << PUSH_BUTTON)) ? 1 : 0;
		if (Last_State == 1 && Current_State == 0)
		{
			PORTB ^= (1 << LED_PIN);
			_delay_ms(200);
		}
		Last_State = Current_State;
    }
}

