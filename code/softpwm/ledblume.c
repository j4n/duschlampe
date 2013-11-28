/*
 * ledblume.c
 *
 *  Created on: Dec 20, 2010
 *      Author: szehe
 *
 *
 *
 *      SOMEHOW WORKS, IT HAS TIMEOUTS AND THE OUTPUT ONLY WORKS IN THE INTERRUPT. FUNNY...
 *
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>
#include "includes/utils.h"
#include "includes/ledblume.h"
#include "includes/display.h"
#include <avr/eeprom.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdint.h>
#include <avr/pgmspace.h>
#ifndef F_CPU
#define F_CPU = 1000000UL
#endif


//gradient maps for idle animation:

static const uint8_t pgmRedGradient[] PROGMEM = {126,128,129,129,130,129,129,128,128,127,127,126,127,127,127,128,128,127,130,128,130,130,129,128,127,126,127,126,127,127,128,129,130,127,129,129,130,127,123,118,114,109,106,102,99,96,95,90,85,82,81,77,75,71,66,62,57,54,50,47,43,40,39,34,31,26,25,23,17,13,10,7,3,0,0,0,0,0,1,1,0,0,0,0,0,1,0,0,0,1,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,1,2,1,2,0,1,0,1,0,0,0,0,0,0,0,0,0,1,2,1,0,0,0,};
static const uint8_t pgmGreenGradient[] PROGMEM = {3,5,8,11,14,18,22,26,29,33,37,42,44,46,49,52,58,61,65,67,69,74,77,81,85,89,92,96,99,102,105,108,115,116,119,123,126,127,128,127,129,128,128,129,128,129,127,127,127,129,128,128,127,127,128,128,128,129,129,129,129,128,127,129,128,129,128,129,129,129,128,128,127,128,128,128,128,128,129,128,128,127,127,127,129,129,129,128,127,128,126,126,124,121,118,116,109,106,103,101,98,94,90,86,82,79,75,72,68,66,62,59,55,52,49,46,43,39,34,30,27,23,19,15,12,9,6,4,};
static const uint8_t pgmBlueGradient[] PROGMEM = {0,0,0,0,1,0,2,1,0,0,0,0,0,0,0,0,0,0,1,0,2,0,1,0,0,0,0,0,0,1,1,1,0,0,0,1,3,3,2,0,2,0,1,0,0,0,0,0,1,1,0,0,0,0,1,4,6,10,14,18,22,24,25,29,35,37,39,41,45,48,54,57,64,67,71,74,77,79,81,85,90,94,97,98,101,106,109,113,116,121,124,128,129,129,127,128,129,128,128,126,129,128,127,129,129,128,130,128,129,127,127,127,129,128,128,126,128,127,126,128,127,127,129,127,128,126,126,124,};


/*
static const uint8_t pgmRedGradient[] PROGMEM = {128,128,127,128,127,128,129,128,126,128,127,127,127,127,126,126,126,126,125,125,122,116,111,106,100,94,88,85,81,77,74,67,59,52,44,39,29,25,26,24,22,24,22,22,21,20,19,20,18,18,17,16,14,12,10,10,8,4,1,2,1,3,10,15,21,26,30,34,37,40,43,44,47,48,51,54,56,60,63,64,67,70,71,72,74,78,80,83,87,89,92,93,97,97,101,102,105,106,107,111,115,117,120,124,124,125,128,127,127,126,128,128,128,127,127,127,127,128,126,127,127,128,128,127,128,127,128,127,};
static const uint8_t pgmGreenGradient[] PROGMEM = {29,30,31,32,33,34,36,37,38,40,40,41,43,44,45,47,49,50,51,51,51,52,53,52,52,54,55,55,55,55,57,55,58,56,59,59,58,60,61,64,66,70,71,75,77,81,82,85,87,90,91,92,94,97,100,102,107,109,111,112,114,113,114,115,112,110,111,110,109,109,108,107,107,107,106,106,104,103,103,103,103,102,101,102,101,101,100,100,99,102,99,98,98,98,97,97,97,97,96,95,95,96,95,94,94,93,94,91,88,85,82,77,74,68,64,62,61,59,55,51,46,42,38,34,30,27,26,25,};
static const uint8_t pgmBlueGradient[] PROGMEM = {24,29,35,43,49,58,65,72,76,80,83,88,93,98,104,112,117,124,126,126,127,128,129,128,126,127,126,129,128,127,128,127,128,129,128,128,126,126,129,126,127,130,129,127,126,128,126,127,126,130,128,126,127,128,127,127,130,128,128,129,128,126,123,117,115,110,104,100,97,91,88,86,81,77,74,68,64,58,51,46,42,37,31,32,32,33,31,32,33,33,32,32,32,32,33,33,32,32,32,33,32,31,31,32,32,32,33,31,31,31,32,30,30,28,29,30,29,28,27,27,25,27,27,26,27,25,22,23,};
*/

/*
static const uint8_t pgmRedGradient[] PROGMEM = { 29, 29, 29, 30, 28, 29, 29,
		29, 30, 29, 29, 29, 28, 29, 30, 29, 30, 30, 31, 30, 31, 30, 27, 26, 25,
		24, 22, 20, 20, 18, 17, 16, 14, 12, 11, 10, 8, 8, 6, 5, 4, 2, 1, 1, 0,
		0, 0, 0, 1, 1, 2, 2, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0,
		0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 2, 1, 5, 5, 6, 7, 9,
		10, 11, 13, 15, 16, 19, 19, 20, 21, 23, 23, 25, 26, 29, 30, 31, 31, 31,
		30, 30, 30, 29, 29, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, };
static const uint8_t
		pgmGreenGradient[] PROGMEM = { 4, 4, 6, 7, 8, 9, 11, 11, 14, 15, 16,
				17, 20, 21, 22, 23, 24, 26, 27, 29, 30, 30, 30, 31, 30, 31, 31,
				31, 31, 31, 30, 31, 31, 31, 32, 31, 32, 32, 31, 32, 31, 31, 32,
				32, 32, 32, 32, 32, 31, 30, 31, 31, 31, 31, 31, 31, 31, 31, 31,
				31, 31, 31, 32, 31, 29, 29, 28, 27, 25, 24, 22, 22, 19, 19, 17,
				16, 14, 13, 11, 11, 9, 8, 6, 5, 3, 3, 1, 0, 0, 0, 0, 0, 0, 1,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0,
				0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, };
static const uint8_t
		pgmBlueGradient[] PROGMEM = { 0, 0, 0, 1, 1, 2, 1, 1, 1, 2, 0, 1, 1, 2,
				3, 1, 0, 0, 0, 0, 2, 2, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1,
				0, 0, 0, 1, 1, 0, 0, 1, 1, 3, 3, 3, 5, 7, 8, 11, 11, 13, 13,
				16, 16, 21, 21, 23, 23, 25, 25, 27, 29, 30, 32, 32, 31, 32, 31,
				30, 30, 33, 33, 32, 31, 31, 30, 31, 31, 32, 31, 32, 31, 30, 30,
				32, 31, 32, 32, 30, 31, 31, 32, 30, 30, 31, 31, 30, 30, 30, 31,
				29, 29, 30, 31, 30, 31, 29, 29, 29, 26, 24, 23, 22, 20, 20, 18,
				16, 15, 15, 11, 11, 9, 6, 6, 6, 6, };
*/



//testvars
/*
 uint8_t col_1[8] = { 0, 1, 2, 3, 4, 5, 6, 7 };
 uint8_t col_2[8] = { 1, 2, 3, 4, 5, 6, 7, 0 };
 uint8_t col_3[8] = { 2, 3, 4, 5, 6, 7, 0, 1 };
 uint8_t counttest = 0;
 uint16_t count = 0;
 */

//variables for software-pwm
uint8_t reds[2] = { 0, 0 }; //{ LED1, LED2, LED3} 0-32
uint8_t greens[2] = { 0, 0 }; //{ LED1, LED2, LED3} 0-32
uint8_t blues[2] = { 0, 0}; //{ LED1, LED2, LED3} 0-32
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
	TCCR0B |= (1 << CS00); // Prescaler 0
	// Compare Interrupt auf A erlauben
	TCCR0A |= (1 << WGM01);
	TIMSK0 |= (1 << OCIE0A);
	// set Flag on Compare Interrupt
	TIFR0 |= (1 << OCF0A);
	// Set Timer Compare Value
	OCR0A = 50;

	// Timer 1 konfigurieren
	//ein interrupt pro millisekunde@20mhz
	TCCR1B = (1 << CS10) | (1 << WGM12); // Prescaler 1024
	OCR1AH = 0xFF;
	OCR1AL = 0xFF;
	TIMSK1 = (1 << OCIE1A);

	//configure outputs
	//DDRD |= (1 << PD0) | (1 << PD1) | (1 << PD2);
	//DDRC |= (1 << PC1) | (1 << PC2) | (1 << PC3);
	DDRB |= (1 << PB1) | (1 << PB2) | (1 << PB3);

	//enable interrupts
	sei();

while(1) {/*

		PORTD &= ~(1 << PD0); //set R-Chan to 0

		PORTD &= ~(1 << PD2); //set G-Chan to 0

		PORTD &= ~(1 << PD1); //set B-Chan to 0

		PORTC &= ~(1 << PC1); //set R-Chan to 0

		PORTC &= ~(1 << PC2); //set G-Chan to 0

		PORTC &= ~(1 << PC3); //set B-Chan to 0
*/	
}


}

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
		reds[0] =  pgm_read_byte(&pgmRedGradient[animation0/2]);
		greens[0] =  pgm_read_byte(&pgmGreenGradient[animation0/2]);
		blues[0] =  pgm_read_byte(&pgmBlueGradient[animation0/2]);

		reds[1] =  pgm_read_byte(&pgmRedGradient[animation1/2]);
		greens[1] =  pgm_read_byte(&pgmGreenGradient[animation1/2]);
		blues[1] =  pgm_read_byte(&pgmBlueGradient[animation1/2]);
	}
}

ISR (TIMER0_COMPA_vect)
{

	//compare all of em pwm-values to counter
//	if (pwmcounter > reds[0]) {
//		PORTD |= (1 << PD0); //set R-Chan to 0
//	}
//	if (pwmcounter > greens[0]) {
//		PORTD |= (1 << PD1); //set G-Chan to 0
//	}
//	if (pwmcounter > blues[0]) {
//		PORTD |= (1 << PD2); //set B-Chan to 0
//	}
//	if (pwmcounter > reds[1]) {
//		PORTC |= (1 << PC3); //set R1-Chan to 0
//	}
//	if (pwmcounter > greens[1]) {
//		PORTC |= (1 << PC2); //set G1-Chan to 0
//	}
//	if (pwmcounter > blues[1]) {
//		PORTC |= (1 << PC1); //set B1-Chan to 0
//	}
	if (pwmcounter > reds[1]) {
		PORTB |= (1 << PB3); //set R1-Chan to 0
	}
	if (pwmcounter > greens[1]) {
		PORTB |= (1 << PB2); //set G1-Chan to 0
	}
	if (pwmcounter > blues[1]) {
		PORTB |= (1 << PB1); //set B1-Chan to 0
	}
	//inc pwm-counter. this is used to compare the pwm-values for each channel and toggle the ports on match.
	pwmcounter++;
	//if pwmcounter = 0, reset all chans to 1
	if (pwmcounter >128) {

		PORTB &= ~((1 << PB1) | (1 << PB2) | (1 << PB3));
		//PORTC &= ~((1 << PC1) | (1 << PC2) | (1 << PC3));
		//PORTD &= ~((1 << PD0) | (1 << PD1) | (1 << PD2));
		pwmcounter = 0;
		
	}
}
