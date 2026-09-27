#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>

#define BAUD 9600
#define Scaled_Factor 320312UL

// Function Prototypes
void UART_Init(void);
void UART_SendChar(char data);
int  UART_putchar(char c, FILE *stream);


void ADC_Init(){
	ADMUX |= (1 << REFS0);												// Reference Selection
	ADMUX &= ~((1 << MUX0) | (1 << MUX1) | (1 << MUX2));				// Selecting the ADC channel
	ADCSRA |= (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);				// ADC Prescaler (128 factor)
	ADCSRB &= ~((1 << ADTS2) | (1 << ADTS1) | (1 << ADTS0));			// Triggering Source (Free Running)
	DIDR0 |= (1 << ADC0D);												// Disabling Digital Input
	ADCSRA |= (1 << ADEN);												// Enabling the ADC
}

uint16_t ADC_Read(){
	uint16_t result;
	ADCSRA |= (1 << ADSC);												// Start ADC Conversion
	while(ADCSRA & (1 << ADSC));										// Wait for the result
	// ADC Result
	//return ADC;
	result = ADCL;
	result |= ((uint16_t)ADCH << 8);
	return result;
}


static FILE uart_output = FDEV_SETUP_STREAM(UART_putchar, NULL, _FDEV_SETUP_WRITE);
void UART_Init(){
	//set the baud rate
	uint16_t ubrr = (F_CPU/(16UL*BAUD))-1;
	UBRR0L = (uint8_t)ubrr;
	UBRR0H = (uint8_t)(ubrr >> 8);
	//frame format
	UCSR0C &= ~((1 << UMSEL00) | (1 << UMSEL01));						// USART Mode Select (Asynchronous)
	UCSR0C &= ~((1 << UPM00) | (1 << UPM01));							// Parity Mode (Disable)
	UCSR0C &= ~((1 << USBS0));											// Stop Bit (1-bit)
	UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);							// Character Size (8-bit)
	UCSR0C &= ~(1 << UCPOL0);
	//enable the transmitter or receiver
	UCSR0B |= (1 << TXEN0);
	stdout = &uart_output;
}

void UART_SendChar(char data){
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = data;
}

int UART_putchar(char c, FILE *stream){
	if (c == '\n'){
	UART_SendChar('\r');
	}
	UART_SendChar(c);
	return 0;
}




int main(void)
{
	uint16_t adc_result;
	uint8_t eight_bit;
	uint8_t four_bit;
	uint16_t millivolt;
	
	ADC_Init();
	UART_Init();

    while (1) 
    {
		adc_result = ADC_Read();
		eight_bit = (uint8_t)(adc_result >> 2);
		four_bit = (eight_bit >> 4);
		millivolt = (uint16_t)(((uint32_t)adc_result * Scaled_Factor) >> 16);
		
		printf("ADC_10_bit = %u	->	ADC_8_bit = %u	->	ADC_4_bit = %u	->	mV = %u\n", adc_result, eight_bit, four_bit, millivolt);
		_delay_ms(2000);
    }
}

