// vim: ft=c:ts=2:sw=2
// range y: 300-350

#include <avr/interrupt.h>

uint8_t counter = 0;
uint8_t r = 255;
uint8_t g = 128;
uint8_t b = 255;

int main(void) {
	TCCR0 |= (1 << CS00); // prescaler
	TIMSK |= (1 << TOIE0);
	// Configure outputs
	DDRB |= (1 << PB1) | (1 << PB2) | (1 << PB3);
	//pinMode(11, OUTPUT); // 11 = PB3
	sei();
	
	while (1) { //main loop

	}
}

ISR (TIMER0_OVF_vect) {
	PORTB ^= (1 << PB3);
	counter++;

	if (counter > r) PORTB &= !(1 << PB1);
	if (counter > g) PORTB &= !(1 << PB2);
	if (counter > b) PORTB &= !(1 << PB3);

	if(counter >= 255) {
		if (r > 0) PORTB |= (1 << PB1);
		if (g > 0) PORTB |= (1 << PB2);
		if (b > 0) PORTB |= (1 << PB3);
		counter = 0;
	}
}
