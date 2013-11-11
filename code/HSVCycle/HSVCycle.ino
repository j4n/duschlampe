/* vim:ft=c:expandtab:nocindent:ts=2:sw=2
HSV fade/bounce for Arduino - Stewart C. Russell - scruss.com - 2010/09/19

Wiring:
LED is RGB common cathode (SparkFun sku: COM-09264 or equivalent)
    * Digital pin  9 → 165Ω resistor → LED Red pin
    * Digital pin 10 → 100Ω resistor → LED Green pin
    * Digital pin 11 → 100Ω resistor → LED Blue pin
    * GND → LED common cathode.
*/

#define DELAY  1
#define STROBE 1

const int ledPins[3] = { 1, 0, 4 };

long rgb[3];
long rgbval, k;
float hsv[3] = {
  0.0, 0.5, 0.5
};
float hsv_min[3] = {
  0.0, 0.0, 0.4 // keep V term greater than 0 for smoothness
};
float hsv_max[3] = {
  6.0, 1.0, 1.0
};
float hsv_delta[3] = {
  0.0005, 0.00013, 0.00011
};

/*
chosen LED SparkFun sku: COM-09264
 has Max Luminosity (RGB): (2800, 6500, 1200)mcd
 so we normalize them all to 1200 mcd -
 R  1200/2800  =  0.428571428571429   =   109/256
 G  1200/6500  =  0.184615384615385   =    47/256
 B  1200/1200  =  1.0                 =   256/256
 */
long bright[3] = {
  109, 47, 256
};

void setup () {
  PWM4_init();

  randomSeed(analogRead(4));
  for (k=0; k<3; k++) {
    pinMode(ledPins[k], OUTPUT);
    rgb[k]=0; // start with the LED off
    analogWriteWrap(ledPins[k], rgb[k] * bright[k]/256);
    if (k>1 && random(100) > 50) {
      // randomly twiddle direction of saturation and value increment on startup
      hsv_delta[k] *= -1.0;
    }
  }
}

//void loop() {
//  analogWriteWrap(1, 128);
//}
void loop() {
  for (k=0; k<3; k++) { // for all three HSV values
    hsv[k] += hsv_delta[k];
    if (k<1) { // hue sweeps simply upwards
      if (hsv[k] > hsv_max[k]) {
        hsv[k]=hsv_min[k];
      }    
    }
    else { // saturation or value bounce around
      if (hsv[k] > hsv_max[k] || hsv[k] < hsv_min[k]) {
        hsv_delta[k] *= -1.0;
        hsv[k] += hsv_delta[k];
      }
    }
    hsv[k] = constrain(hsv[k], hsv_min[k], hsv_max[k]); // keep values in range
  }

  rgbval=HSV_to_RGB(hsv[0], hsv[1], hsv[2]);
  rgb[0] = (rgbval & 0x00FF0000) >> 16; // there must be better ways
  rgb[1] = (rgbval & 0x0000FF00) >> 8;
  rgb[2] = rgbval & 0x000000FF;

  for (k=0; k<3; k++) { // for all three RGB values
    analogWriteWrap(ledPins[k], rgb[k] * bright[k]/256);
  }
  delay(DELAY);
}

long HSV_to_RGB( float h, float s, float v ) {
  /*
     modified from Alvy Ray Smith's site:
   http://www.alvyray.com/Papers/hsv2rgb.htm
   H is given on [0, 6]. S and V are given on [0, 1].
   RGB is returned as a 24-bit long #rrggbb
   */
  int i;
  float m, n, f;

  // not very elegant way of dealing with out of range: return black
  if ((s<0.0) || (s>1.0) || (v<0.0) || (v>1.0)) {
    return 0L;
  }

  if ((h < 0.0) || (h > 6.0)) {
    return long( v * 255 ) + long( v * 255 ) * 256 + long( v * 255 ) * 65536;
  }
  i = floor(h);
  f = h - i;
  if ( !(i&1) ) {
    f = 1 - f; // if i is even
  }
  m = v * (1 - s);
  n = v * (1 - s * f);
  switch (i) {
  case 6:
  case 0: // RETURN_RGB(v, n, m)
    return long(v * 255 ) * 65536 + long( n * 255 ) * 256 + long( m * 255);
  case 1: // RETURN_RGB(n, v, m) 
    return long(n * 255 ) * 65536 + long( v * 255 ) * 256 + long( m * 255);
  case 2:  // RETURN_RGB(m, v, n)
    return long(m * 255 ) * 65536 + long( v * 255 ) * 256 + long( n * 255);
  case 3:  // RETURN_RGB(m, n, v)
    return long(m * 255 ) * 65536 + long( n * 255 ) * 256 + long( v * 255);
  case 4:  // RETURN_RGB(n, m, v)
    return long(n * 255 ) * 65536 + long( m * 255 ) * 256 + long( v * 255);
  case 5:  // RETURN_RGB(v, m, n)
    return long(v * 255 ) * 65536 + long( m * 255 ) * 256 + long( n * 255);
  }
} 

void analogWriteWrap(uint8_t pin, uint8_t duty) {
  if (pin == 4) {
    analogWrite4(duty);
  } else {
    analogWrite(pin, duty);
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
