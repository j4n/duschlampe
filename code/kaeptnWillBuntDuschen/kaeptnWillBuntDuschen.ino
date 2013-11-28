/* vim:ft=c
 */

#include <avr/pgmspace.h>
#ifndef F_CPU
#define F_CPU = 1000000UL
#endif


//gradient maps for idle animation:

static const uint8_t pgmRedGradient[] PROGMEM = {126,128,129,129,130,129,129,128,128,127,127,126,127,127,127,128,128,127,130,128,130,130,129,128,127,126,127,126,127,127,128,129,130,127,129,129,130,127,123,118,114,109,106,102,99,96,95,90,85,82,81,77,75,71,66,62,57,54,50,47,43,40,39,34,31,26,25,23,17,13,10,7,3,0,0,0,0,0,1,1,0,0,0,0,0,1,0,0,0,1,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,1,2,1,2,0,1,0,1,0,0,0,0,0,0,0,0,0,1,2,1,0,0,0,};
static const uint8_t pgmGreenGradient[] PROGMEM = {3,5,8,11,14,18,22,26,29,33,37,42,44,46,49,52,58,61,65,67,69,74,77,81,85,89,92,96,99,102,105,108,115,116,119,123,126,127,128,127,129,128,128,129,128,129,127,127,127,129,128,128,127,127,128,128,128,129,129,129,129,128,127,129,128,129,128,129,129,129,128,128,127,128,128,128,128,128,129,128,128,127,127,127,129,129,129,128,127,128,126,126,124,121,118,116,109,106,103,101,98,94,90,86,82,79,75,72,68,66,62,59,55,52,49,46,43,39,34,30,27,23,19,15,12,9,6,4,};
static const uint8_t pgmBlueGradient[] PROGMEM = {0,0,0,0,1,0,2,1,0,0,0,0,0,0,0,0,0,0,1,0,2,0,1,0,0,0,0,0,0,1,1,1,0,0,0,1,3,3,2,0,2,0,1,0,0,0,0,0,1,1,0,0,0,0,1,4,6,10,14,18,22,24,25,29,35,37,39,41,45,48,54,57,64,67,71,74,77,79,81,85,90,94,97,98,101,106,109,113,116,121,124,128,129,129,127,128,129,128,128,126,129,128,127,129,129,128,130,128,129,127,127,127,129,128,128,126,128,127,126,128,127,127,129,127,128,126,126,124,};

//variables for software-pwm
uint8_t red = 64 ;
uint8_t green = 2 ;
uint8_t blue = 30;
uint8_t pwmcounter = 0;
uint8_t ledcounter = 0;

//variables for openline animation
uint8_t openline = 1;
uint8_t animation0 = 0;
uint8_t animation1 = 42;
int8_t dir1 = -1;
int8_t dir2 = 1;
;

//variables for resistor measurement
uint16_t milliseconds = 1;
char wert[8];
uint8_t status = 0; //0=keine Messung, 1=Messung erfolgreich, 2=Out of Bounds
uint8_t firstiteration = 1; //first iteration of the loop?
uint8_t displayactive = 0;

int main(void) {
	// Timer 0 konfigurieren
	TCCR2 |= (1 << CS20); // Prescaler 0
	// Compare Interrupt auf A erlauben
	TCCR2 |= (1 << WGM21);
	TIMSK |= (1 << OCIE2);
	// set Flag on Compare Interrupt
	TIFR |= (1 << OCF2);
	// Set Timer Compare Value
	OCR2 = 50;
/*
	// Timer 1 konfigurieren
	//ein interrupt pro millisekunde@20mhz
	TCCR1B = (1 << CS10) | (1 << WGM12); // Prescaler 1024
	OCR1AH = 0xFF;
	OCR1AL = 0xFF;
	TIMSK = (1 << OCIE1A);
*/
	//configure outputs
	DDRB |= (1 << PB1) | (1 << PB2) | (1 << PB3);

	//enable interrupts
	sei();

	while(1) {}
}

/*
ISR (TIMER1_COMPA_vect)
{
	if (openline) {
		animation0 += dir1;
		animation1 += dir2;
		if (animation0 == 255) dir1 = -1;
		if (animation0 == 0) dir1 = 1;
		if (animation1 == 255) dir2 = -1;
		if (animation1 == 0) dir2 = 1;
		//if (animation1 > 129) animation1 = 0;
//		red =  pgm_read_byte(&pgmRedGradient[animation0/2]);
//		green =  pgm_read_byte(&pgmGreenGradient[animation0/2]);
//		blue =  pgm_read_byte(&pgmBlueGradient[animation0/2]);
		red =  64;
		green =  64;
		blue =  64;
	}
}*/

ISR (TIMER2_COMP_vect)
{

	//compare all of em pwm-values to counter
	if (pwmcounter > red) {
		PORTB |= (1 << PB3); //set R1-Chan to 0
	}
	if (pwmcounter > green) {
		PORTB |= (1 << PB2); //set G1-Chan to 0
	}
	if (pwmcounter > blue) {
		PORTB |= (1 << PB1); //set B1-Chan to 0
	}
	//inc pwm-counter. this is used to compare the pwm-values for each channel and toggle the ports on match.
	pwmcounter++;
	//if pwmcounter = 0, reset all chans to 1
	if (pwmcounter > 128) {

		PORTB &= ~((1 << PB1) | (1 << PB2) | (1 << PB3));
		//PORTC &= ~((1 << PC1) | (1 << PC2) | (1 << PC3));
		//PORTD &= ~((1 << PD0) | (1 << PD1) | (1 << PD2));
		pwmcounter = 0;
		
	}
}

