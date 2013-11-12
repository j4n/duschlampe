/* 
 * vim:ft=c:expandtab:nocindent:ts=2:sw=2 
 */

#include <avr/pgmspace.h>

//////////////////////////////////////////////////////
// RainbowLED,                                      //
// User Application,                                //
// This example demonstrates how use analogWrite to //
// control the brightness of an LED                 //
//////////////////////////////////////////////////////

// define LED pins
#define LED_RED      1
#define LED_GREEN    0
#define LED_BLUE     4

// A gradient for each channel
PROGMEM  prog_uint8_t gradientRed [] = {255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,242,229,216,204,191,178,165,153,140,127,114,101,89,76,63,50,38,25,12,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,12,25,38,51,63,76,89,102,114,127,140,152,165,178,191,203,216,229,242,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255};
PROGMEM  prog_uint8_t gradientGreen [] = {0,12,25,38,50,63,76,89,102,114,127,140,153,165,178,191,204,216,229,242,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,242,229,216,203,191,178,165,153,140,127,114,101,89,76,63,51,38,25,12,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
PROGMEM  prog_uint8_t gradientBlue [] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,12,25,38,51,63,76,89,101,114,127,140,153,165,178,191,203,216,229,242,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,242,229,216,203,191,178,165,152,140,127,114,102,89,76,63,51,38,25,12,0};
//PROGMEM  prog_uint8_t gradientRed [] = { 255,254,255,255,254,255,255,253,254,254,255,255,255,253,251,248,244,239,233,226,223,216,207,200,191,183,174,163,155,146,136,125,117,106,95,86,76,68,59,51,46,37,31,25,20,14,11,6,3,1,0,0,1,0,0,0,1,0,0,0,0,1,0,2,5,10,13,17,22,29,33,41,49,57,65,74,82,91,100,111,121,130,141,150,160,169,178,187,195,204,210,216,224,230,234,240,244,249,252,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,254,255,255,255,255,255,254,255};
//PROGMEM  prog_uint8_t gradientGreen [] = { 19,21,22,24,26,29,32,35,37,40,41,47,50,51,56,59,61,64,67,70,74,77,80,83,85,90,94,96,100,103,108,112,116,120,125,130,135,139,145,150,155,160,164,171,177,182,188,195,200,205,210,215,221,225,230,234,238,242,245,248,251,253,255,255,255,255,255,255,255,255,255,254,254,255,255,254,255,254,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,254,253,249,245,241,238,233,229,223,217,212,205,197,190,183,175,168,159,151,142,135,126,117,109,100,92,85,76,69,61,56,49,43,38};
//PROGMEM  prog_uint8_t gradientBlue []= { 67,76,85,94,103,113,123,132,142,152,162,171,181,189,197,205,213,219,225,232,236,242,245,250,253,255,255,254,254,255,255,254,254,255,255,255,255,255,255,254,255,254,255,255,254,255,255,253,255,255,255,254,255,255,254,255,255,253,250,247,242,238,233,226,220,212,205,196,189,179,168,161,149,139,129,121,109,99,89,80,72,60,54,46,39,32,25,20,17,12,6,4,3,1,0,0,0,0,0,0,0,0,0,0,1,0,0,2,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0};

byte gradientIndex = 0;
int singleDelay = 1;

void setup(){
  PWM4_init();

  // setup LED pins
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  // set LED pins high, all LEDs off
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_BLUE, LOW);
}

void loop(){
  for (int red=0; red < 256; red ++){
    analogWrite(LED_RED, red);
    delay(singleDelay);
  }

  for (int blue=0; blue < 256; blue ++){
    analogWrite4(blue);
    delay(singleDelay);
  }

  for (int green=0; green < 256; green ++){
    analogWrite(LED_GREEN, green);
    delay(singleDelay);
  }

  for (int red=255; red >= 0; red --){
    analogWrite(LED_RED, red);
    delay(singleDelay);
  }

  for (int green=255; green >= 0; green --){
    analogWrite(LED_GREEN, green);
    delay(singleDelay);
  }

  for (int blue=255; blue >= 0; blue --){
    analogWrite4(blue);
    delay(singleDelay);
  }
 
  // Rainbow fade forever
  while(true){
    // Write inversed RGB-Values at index position
    analogWrite(LED_RED, pgm_read_word_near(gradientRed + gradientIndex));
    analogWrite(LED_GREEN, pgm_read_word_near(gradientGreen + gradientIndex));
    analogWrite4(pgm_read_word_near(gradientBlue + gradientIndex));
    // Increase index and handle overflow
    gradientIndex ++;
    if (gradientIndex > sizeof(gradientRed)-1){
      gradientIndex = 0;
    }
    // Wait for a short time
    delay(100);
  }
}


void PWM4_init() {
  // Set up PWM on Trinket GPIO #4 (PB4, pin 3) using Timer 1
  TCCR1 = _BV (CS10);           // no prescaler
  GTCCR = _BV (COM1B1) | _BV (PWM1B);  //  clear OC1B on compare
  OCR1B = 127;                  // duty cycle initialize to 50%
  OCR1C = 255;                  // frequency
}
 
// Function to allow analogWrite on Trinket GPIO #4 
void analogWrite4(uint8_t duty_value) {  
  OCR1B = duty_value;  // duty may be 0 to 255 (0 to 100%)
}
