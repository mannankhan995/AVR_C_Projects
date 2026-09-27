#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>

#define LED_PIN			PORTB5
#define PUSH_BUTTON		PORTB0
volatile uint8_t Last_State = 1;
volatile uint8_t debounce_counter = 0;

void Init_Timer(){
    TCCR0A |= (1 << WGM01);										// CTC (Clear Timer on Compare Match) mode
    TCCR0B |= (1 << CS02) | (1 << CS00);						// Prescaler = 1024 
    OCR0A = 15;													// Compare value for 1 ms
    TIMSK0 |= (1 << OCIE0A);									// Enable Timer1 Compare Match A interrupt
	sei();
}

ISR(TIMER0_COMPA_vect){
	uint8_t Current_State = (PINB & (1 << PUSH_BUTTON)) ? 1 : 0;
	if (Last_State == 1 && Current_State == 0 && debounce_counter == 10)
	{
		PORTB ^= (1 << LED_PIN);
		debounce_counter = 0;
	}
	if (debounce_counter < 10)
	{
		debounce_counter++;
	}
	Last_State = Current_State;
}

int main(void)
{
	DDRB |= (1 << LED_PIN);										// Setting the pin 13 as output
	DDRB &= ~(1 << PUSH_BUTTON);								// Setting the pin 8 as Input
	PORTB |= (1 << PUSH_BUTTON);								// pull up
	Init_Timer();
	while (1)
	{
	}
}


