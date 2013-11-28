// vim: ft=c:ts=2:sw=2
// range y: 300-350
#define UART_BAUD_RATE 9600

#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>
#include "uart/uart.h"

uint8_t counter = 0;
uint8_t r = 0;
uint8_t g = 0;
uint8_t b = 0;

uint16_t ay = 0;

char buffer[7];
uint8_t isOn;
uint8_t interval = 128;

uint16_t readADC(uint8_t channel);
long map(long x, long in_min, long in_max, long out_min, long out_max);

int main(void) {
	// init stuff
	// Configure outputs
	DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2);

//	// Timer0 for Software PWM
//	TCCR0 |= (1 << CS00); // prescaler
//	TIMSK |= (1 << TOIE0); // timer overflow interrupt

	// Timer1 for Software PWM
	TCCR1B |= (1 << WGM12);
	//TCCR1B |= (1 << WGM13);
	TCCR1B |= (1 << CS10);
	TIMSK |= (1 << OCIE1A);
	OCR1A = 150;

	// Timer 2 for Strobing, compare value in OCR2
	TCCR2 |= (1 << WGM21); // wave generation mode
	TCCR2 |= (1 << CS21); // prescaler clk/8
	TIMSK |= (1 << OCIE2); // match & compare interrupt
	OCR2 = 128;
	
	sei();
	
	uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU) ); 
	
	// end init

	while (1) { //main loop
		//ay = readADC(1);
		//r = map(ay, 570, 700, 0, 255);
		r ++;
		_delay_ms(20);
		//r = map(ay, 450, 700, 0, 255);
    //itoa( ay, buffer, 10);   // convert interger into string (decimal format)
    //uart_puts(buffer);        // and transmit string to UART
		//uart_putc('/');
    //itoa( r, buffer, 10);   // convert interger into string (decimal format)
    //uart_puts(buffer);        // and transmit string to UART
		//uart_putc('\n');
	}
}

ISR (TIMER0_OVF_vect) {
	PORTB ^= (1 << PB0);
	/*
	counter++;

	if (counter > g) PORTB &= ~(1 << PB0);
	if (counter > b) PORTB &= ~(1 << PB1);
	if (counter > r) PORTB &= ~(1 << PB2);

	if(counter >= 255) {
		if (g > 0) PORTB |= (1 << PB0);
		if (b > 0) PORTB |= (1 << PB1);
		if (r > 0) PORTB |= (1 << PB2);
		counter = 0;
	}
	*/
}

ISR (TIMER1_COMPA_vect) {
	//PORTB ^= (1 << PB0);

	counter++;

	if (counter > g) PORTB &= ~(1 << PB0);
	if (counter > b) PORTB &= ~(1 << PB1);
	if (counter > r) PORTB &= ~(1 << PB2);

	if(counter >= 255) {
		if (g > 0) PORTB |= (1 << PB0);
		if (b > 0) PORTB |= (1 << PB1);
		if (r > 0) PORTB |= (1 << PB2);
		counter = 0;
	}
}

ISR (TIMER2_COMP_vect) {
  //PORTB ^=  (1<<PB0);
	
	if (isOn) {
    //analogWrite(redPin, intensity);
		//TCCR0 |= (1 << CS00); // restart timer0
    TIMSK |= (1 << OCIE1A);
		
		//r = 0;
		OCR2 = 80;
    isOn = 0;
  }
  else {
		TIMSK &= ~(1 << OCIE1A);
		//PORTB &= ~(1 << PB0);
		//PORTB &= ~(1 << PB1);
		//PORTB &= ~(1 << PB2);
    //r = 255;
    //digitalWrite(redPin,LOW);
    OCR2 = interval;
		//TCCR0 = 0; // stop timer0
    isOn = 1;
  }
}

uint16_t readADC(uint8_t channel) {
	uint8_t i;
	uint16_t result = 0;

	// Activate ADC, divider to 64
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1);

	// Select ADC channel
	// Use internal voltage reference (2,56 V)
	ADMUX = channel | (1 << REFS1) | (1 << REFS0);

	// Init ADC, perform dummy readout
	ADCSRA |= (1 << ADSC);
	while (ADCSRA & (1 << ADSC))
		;

	// 3 measurements per channel
	// Calculate average
	for (i = 0; i < 3; i++) {
		// Measure
		ADCSRA |= (1 << ADSC);
		// Wait for result...
		while (ADCSRA & (1 << ADSC))
			;
		result += ADCW;
	}

	// Deactivate ADC
	ADCSRA &= ~(1 << ADEN);

	result /= 3;

	return result;
}

long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
