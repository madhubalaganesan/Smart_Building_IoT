float tempC;
int reading;
int tempPin = 0;

int redLED = 2;
int greenLED = 3;

void setup()
{
analogReference(INTERNAL);
Serial.begin(9600);

// LED pins
pinMode(redLED, OUTPUT);  // RED LED
pinMode(greenLED, OUTPUT);  // GREEN LED

}

void loop()
{
  reading = analogRead(tempPin);
  tempC = reading / 9.31;
  Serial.println(tempC);
  
  if (tempC < 29.9) {
    digitalWrite(greenLED, HIGH);
    digitalWrite(redLED, LOW);
    delay(1000);     
  } else if (tempC >= 29.9 && tempC < 30) {
    rBlinkSlowly(); 
    }
  else {
    rBlinkFast();
   } 
}


void rBlinkSlowly() {
    digitalWrite(greenLED, LOW);
    digitalWrite(redLED, HIGH);
    delay(1000);  
    digitalWrite(redLED, LOW);
    delay(1000);       
}

void rBlinkFast() {
    digitalWrite(greenLED, LOW);
    digitalWrite(redLED, HIGH);
    delay(300);  
    digitalWrite(redLED, LOW);
    delay(300);     
}


