/* vim:ft=c:ts=2:sw=2
 */

#include <avr/pgmspace.h>
#ifndef F_CPU
#define F_CPU = 1000000UL
#endif


//gradient maps for idle animation:

PROGMEM  prog_uint8_t pgmRedGradient[] = {255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,242,229,216,204,191,178,165,153,140,127,114,101,89,76,63,50,38,25,12,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,12,25,38,51,63,76,89,102,114,127,140,152,165,178,191,203,216,229,242,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255};
PROGMEM  prog_uint8_t pgmGreenGradient[] = {0,12,25,38,50,63,76,89,102,114,127,140,153,165,178,191,204,216,229,242,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,242,229,216,203,191,178,165,153,140,127,114,101,89,76,63,51,38,25,12,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
PROGMEM  prog_uint8_t pgmBlueGradient[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,12,25,38,51,63,76,89,101,114,127,140,153,165,178,191,203,216,229,242,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,242,229,216,203,191,178,165,152,140,127,114,102,89,76,63,51,38,25,12,0};

//variables for software-pwm
uint8_t red = 64;
uint8_t green = 2;
uint8_t blue = 30;
uint8_t pwmcounter = 0;
uint8_t ledcounter = 0;

//variables for openline animation
uint8_t openline = 1;
uint8_t animation0 = 0;
uint8_t animation1 = 42;
int8_t dir1 = -1;
int8_t dir2 = 1;

uint8_t gradientIndex = 0;

//variables for resistor measurement
uint16_t milliseconds = 1;
char wert[8];
uint8_t status = 0; //0=keine Messung, 1=Messung erfolgreich, 2=Out of Bounds
uint8_t firstiteration = 1; //first iteration of the loop?
uint8_t displayactive = 0;

void setup() {
	// Timer 0 konfigurieren
	TCCR2 |= (1 << CS20); // Prescaler 0
	// Compare Interrupt auf A erlauben
	TCCR2 |= (1 << WGM21);
	TIMSK |= (1 << OCIE2);
	// set Flag on Compare Interrupt
	TIFR |= (1 << OCF2);
	// Set Timer Compare Value
	OCR2 = 50;

	// Timer 1 konfigurieren
	//ein interrupt pro millisekunde@20mhz
	TCCR1B = (1 << CS10) | (1 << WGM12); // Prescaler 1024
	OCR1AH = 0xFF;
	OCR1AL = 0xFF;
	TIMSK = (1 << OCIE1A);

	//configure outputs
	DDRB |= (1 << PB1) | (1 << PB2) | (1 << PB3);

	//enable interrupts
	sei();
}

void loop() {
//	red =  pgm_read_word_near(pgmRedGradient + gradientIndex)/2;
//	green =  pgm_read_word_near(pgmGreenGradient + gradientIndex)/2;
//	blue =  pgm_read_word_near(pgmBlueGradient + gradientIndex)/2;
////	red =  pgm_read_byte(&pgmRedGradient[gradientIndex/2]);
////	green =  pgm_read_byte(&pgmGreenGradient[gradientIndex/2]);
////	blue =  pgm_read_byte(&pgmBlueGradient[gradientIndex/2]);
//	// Increase index and handle overflow
//	gradientIndex ++;
//	if (gradientIndex > sizeof(pgmRedGradient)-1){
//		gradientIndex = 0;
//	}
//	red =  64;
//	green =  32;
//	blue =  128;
	// Wait for a short time
//	delay(10);
}

/*
ISR (TIMER1_COMPA_vect) {
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

ISR (TIMER2_COMP_vect) {

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
		pwmcounter = 0;
		
	}
}

