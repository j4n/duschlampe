/*
 vim:ft=c:expandtab:nocindent:ts=2
 */

const int redPin = 1;
byte intensity = 0;
boolean isOn = true;
byte interval = 255;
void setup() {
  TCCR1 |= (1<<CTC1);
  TIMSK |= 1<<OCIE1A;  

  //TCCR1 |= (1<<CS13) | (1<<CS12) ;//| (1<<CS11) |(1<<CS10);  
  TCCR1 |= (1<<CS13) | (1<<CS11) |(1<<CS10);  
  sei();
  pinMode(redPin, OUTPUT);
  analogWrite(redPin, 8);
}

ISR(TIMER1_COMPA_vect) {
  //PINB |= 1<<PB1;
  //DDRB ^= (1<<PB1);
  if (isOn){
  analogWrite(redPin, intensity);
  OCR1A=40;
  isOn = false;
  }
  else{
    
    digitalWrite(redPin,LOW);
    OCR1A=interval;
    isOn=true;
  }

}

void loop() {
  //analogWrite(redPin,intensity);

  intensity ++;
  delay(20);


  //delay(2000);
  //digitalWrite(redPin,LOW);
  //delay(1000);
  //analogWrite(redPin,8);
  //while(true){;}

  //  delay(100);
  //  pinMode(redPin, INPUT);
  //  delay(100);
  //  pinMode(redPin, OUTPUT);
}

