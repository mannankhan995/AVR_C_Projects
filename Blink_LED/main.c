#define F_CPU 16000000UL // Define clock speed (16 MHz for Arduino Uno)
#include <avr/io.h>
#include <util/delay.h>

#define LED_PIN PORTB5

int main(void)
{
	DDRB |= (1 << LED_PIN);
	while (1)
	{
		PORTB ^= (1 << LED_PIN);
		_delay_ms(500);
	}
}
