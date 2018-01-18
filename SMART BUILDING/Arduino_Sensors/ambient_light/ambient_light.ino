void setup()
{
  Serial.begin(9600); // open serial port, set the baud rate to 9600 bps
  pinMode(13, OUTPUT);
}
void loop()
{
      int val;
      val=analogRead(0);   //connect grayscale sensor to Analog 0
      Serial.println(val,DEC);//print the value to serial  
      if(val < 100){
        digitalWrite(13, HIGH);
      }
      else{
        digitalWrite(13, LOW);
      }
      delay(100);
}
