/*
 vim:ft=c:expandtab:nocindent:ts=2
 Analog input, serial output
 
 Reads an analog input pin, uses this value as a time delay for a stroboscope.
 Adapted from a sketch by by Steve Krave http://mehax.blogspot.ca/2011/02/arduino-stroboscope.html
 
 The circuit:
 * potentiometer/accelerometer connected to analog pin 0.
   Center pin of the potentiometer goes to the analog pin.
   side pins of the potentiometer go to +5V and ground
 * LEDs connected from digital pins 1,2,3 of Trinket
 


 */

/*
 * connect this:
 * - #0 green led
 * - #1 red led
 * - #2/A1 xin
 * - #3/A3 yin 
 * - #4/A2 blue led
 */

// trinket
const int redPin = 1;
const int grnPin = 0;
const int bluPin = 4;

const int tiltIn = 1;
const int tempIn = 3;

// failduino
//const int greenPin = 0;
//const int redPin = 13;
//const int tiltIn = 5;
//const int tempIn = 4;
//const int bluePin = 4;

float tilt = 1;
float initialTilt = 1;
float hold = 1;              // hold time between iterations (sort of)
float holdFrequency = 1;
unsigned long microsPrev = 0;       // set up value for timing       
unsigned long microsCurrent = 0;    // used for timing purposes
unsigned long millisSerial =0;      //used for serial output timing
float frequency = 1;         // stores frequency
float rpm = 0;              // rpm

void setup() {
  //initialize pins as outputs
  pinMode(tiltIn, INPUT);
//  digitalWrite(xIn, HIGH);
  pinMode(tempIn, INPUT);
//  digitalWrite(yIn, HIGH);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  initialTilt = analogRead(tiltIn);   //take tilt

//  Serial.begin(9600); 
}


void loop() {
  // compute frequency from tilt
  // frequency range: 20-30 Hz (measured steady drops with 25)
  // tilt range: +/- 15 degrees tilt from initialTilt 
  // sensor range: +/- 1g (+/- 90 degrees), leading to 1.25 +/- 0.5V
  // in digital values that's 1024/5*0.75 = 153 to 358.4 maximum range (204 clicks)
  // and 204/180 = 1.1333 per degree
  // for our tilt range (lets assume start at 45 degrees,
  // so half way down from the middle = 254
  // then +/- 15 degrees is 237-271 = 34 clicks
  // 1024/34
  

  // measured values from the accelerometer
  // middle is 315
  // 180 degrees mapped onto 200 clicks (141-341)
  // 1 degree is 1.11111
  // 315 +- (200/180*15)
  
  tilt = analogRead(tiltIn);   //take tilt
  frequency = map(tilt, 298.0, 331.0, 20.0, 30.0);
  
//  // do calculation and scale up to 1024 again so the existing code works for now
//  tilt = analogRead(tiltIn);   //take tilt
//  tilt = (tilt - initialTilt) * 34 + 512 // FIXME
//  tilt -= 237; // substract lower limit

  //hold = tilt*55 + 400; //add some time to scale and for code execution
  hold = map(frequency, 20.0, 30.0, 50000.0, 33333.0); //add some time to scale and for code execution
//  hold = 40000;

  microsCurrent = micros();                // collect current time
  if (microsCurrent > microsPrev + hold){  //set up timing loop
//    frequency = 1000000/(microsCurrent - microsPrev);    
    microsPrev = microsPrev + hold;            //set up millis for delay stuff

    // set LEDs high
    digitalWrite(redPin,HIGH);
    delayMicroseconds(.02*hold);  //leave LEDs on long enough to see

    //set all LEDs low
    digitalWrite(redPin,LOW);
  }

//  Serial.print("sensor = " );                       
//  Serial.print(tiltIn);      
//  Serial.print("\t frequency = ");      
//  Serial.println(frequency);   
}
