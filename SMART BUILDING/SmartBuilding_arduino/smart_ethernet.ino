
#define         MG_PIN                       (2)     //define which analog input channel you are going to use
#define         BOOL_PIN                     (8)     //Arduino D2-CO2 sensor digital pinout, labled with "D" on PCB  
#define         DC_GAIN                      (8.5)   //define the DC gain of amplifier
 
/***********************Software Related Macros************************************/
#define         READ_SAMPLE_TIMES            (10)     //define how many samples you are going to take in normal operation
#define         READ_SAMPLE_INTERVAL         (50)    //define the time interval(in milisecond) between each samples in
//normal operation
 
/**********************Application Related Macros**********************************/
//These values differ from sensor to sensor. User should derermine this value.
#define         ZERO_POINT_X                 (2.602) //lg400=2.602, the start point_on X_axis of the curve
#define         ZERO_POINT_VOLTAGE           (0.324) //define the output of the sensor in volts when the concentration of CO2 is 400PPM
#define         MAX_POINT_VOLTAGE            (0.265) //define the output of the sensor in volts when the concentration of CO2 is 10,000PPM
#define         REACTION_VOLTGAE             (0.059) //define the voltage drop of the sensor when move the sensor from air into 1000ppm CO2
 
/*****************************Globals***********************************************/
float           CO2Curve[3]  =  {ZERO_POINT_X, ZERO_POINT_VOLTAGE, (REACTION_VOLTGAE / (2.602 - 4))};
//Two points are taken from the curve.With these two points, a line is formed which is
//"approximately equivalent" to the original curve. You could use other methods to get more accurate slope
 
//CO2 Curve format:{ x, y, slope};point1: (lg400=2.602, 0.324), point2: (lg10000=4, 0.265)
//slope = (y1-y2)(i.e.reaction voltage)/ x1-x2 = (0.324-0.265)/(log400 - log10000)
int RED = 6;
    int GREEN = 5;
    int BLUE = 9;
    int YELLOW = 7;
    int cFactor = 0;

#include <SPI.h>
#include <Ethernet2.h>
#include <EthernetUdp.h>  

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0x90, 0xA2, 0xDA, 0x10, 0xC0, 0x56
  
};
IPAddress ip(88, 88, 88, 20);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
     pinMode(BOOL_PIN, INPUT);                        //set pin to input
  digitalWrite(BOOL_PIN, HIGH);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }


  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}


void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 1");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          // output the value of each analog input pin
           float val;
    float temp;
    val=analogRead(0);//Connect LM35 on Analog 0
    temp=(500 * val) /1024;
    client.print("Temp:"); //Display the temperature on Serial monitor
    client.print(temp);
    client.println("C");

    int light;
    light=analogRead(1);   //connect grayscale sensor to Analog 0
     client.print("Light:"); 
     client.print(light,DEC);//print the value to serial        
     client.println(" Lux");
    
    delay(500);

    if (temp > 28)
    {
        digitalWrite(GREEN, HIGH);   // turn the GREEN LED on (HIGH is the voltage level)
        digitalWrite(RED, LOW);   // turn the RED LED off (LOW is the voltage level)
         client.println("AC is turned ON");
        delay(2000);              // wait for half a second
       }

else if (temp < 27.5)
{
  digitalWrite(RED, HIGH);   // turn the RED LED on (HIGH is the voltage level)
  digitalWrite(GREEN, LOW);   // turn the GREEN LED off (LOW is the voltage level)
   client.println("Radiator is turned ON");
  delay(2000);              // wait for half a second
}

  else if (temp >= 27.5 && temp <= 28)
  {
    digitalWrite(RED, LOW);   // turn the RED LED off (LOW is the voltage level)
    digitalWrite(GREEN, LOW);   // turn the GREEN LED off (LOW is the voltage level)
     client.println("OPTIMAL TEMPERATURE");
    delay(2000);
     }

     if(light < 200){
       digitalWrite(RED, LOW);   // turn the RED LED off (LOW is the voltage level)
       digitalWrite(GREEN, LOW);   // turn the GREEN LED off (LOW is the voltage level)
       digitalWrite(BLUE, HIGH);   // turn the BLUE LED on (HIGH is the voltage level)
       delay(2000);
     }
     else{
       digitalWrite(RED, LOW);   
       digitalWrite(GREEN, LOW);   
       digitalWrite(BLUE, LOW);   
       delay(2000);
     }
  int percentage;
  float volts;
 
  volts = MGRead(MG_PIN);
  
  percentage = MGGetPercentage(volts, CO2Curve);
   client.print("CO2:");
  if (percentage == -1) {
    client.print("Under heating/beyond range(400~10,000)");
  } else {
     client.print(percentage);
  }
   client.println( "ppm" );
 
  delay(1000);
  if(percentage > 1000 || percentage < 350)
  {
   client.println("CO2 level is abnormal");
    digitalWrite(YELLOW, HIGH);
    delay(2000);
  }
  else{
      client.println("CO2 level is normal");
  }
    digitalWrite(YELLOW, LOW);
     
  
 
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }

/*****************************  MGRead *********************************************
Input:   mg_pin - analog channel
Output:  output of SEN-000007
Remarks: This function reads the output of SEN-000007
************************************************************************************/
float MGRead(int mg_pin) {
  int i;
  float v = 0;
 
  for (i = 0; i < READ_SAMPLE_TIMES; i++) {
    v += analogRead(mg_pin);
    delay(READ_SAMPLE_INTERVAL);
  }
  v = (v / READ_SAMPLE_TIMES) * 5 / 1024 ;
  return v;
}
 
/*****************************  MQGetPercentage **********************************
Input:   volts   - SEN-000007 output measured in volts
         pcurve  - pointer to the curve of the target gas
Output:  ppm of the target gas
Remarks: By using the slope and a point of the line. The x(logarithmic value of ppm)
         of the line could be derived if y(MG-811 output) is provided. As it is a
         logarithmic coordinate, power of 10 is used to convert the result to non-logarithmic
         value.
************************************************************************************/
int  MGGetPercentage(float volts, float *pcurve) {
  volts = volts / DC_GAIN;
  if (volts > ZERO_POINT_VOLTAGE || volts < MAX_POINT_VOLTAGE ) {
    return -1;
  } else {
    return pow(10, (volts - pcurve[1]) / pcurve[2] + pcurve[0]);
    volts = 0;
  }
}
