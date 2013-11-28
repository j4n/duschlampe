// vim: ft=c:ts=2:sw=2

#include <avr/interrupt.h>

int main(void) {
	TCCR0 |= (1 << CS01); // prescaler
	TIMSK |= (1 << TOIE0);
	// Configure outputs
	DDRB |= (1 << PB1) | (1 << PB2) | (1 << PB3);
	//pinMode(11, OUTPUT); // 11 = PB3
	sei();
	
	while (1) {
			//main loop
	}
}

ISR (TIMER0_OVF_vect) {
	//PINB |= (1 << PB3);
	PORTB ^= (1 << PB3);
}
