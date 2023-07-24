/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/

// Include the library
#include <TM1637Display.h>

// Define the connections pins
#define CLK 33
#define DIO 32

// Create a display object of type TM1637Display
TM1637Display display = TM1637Display(CLK, DIO);



/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID "TMPLAodc_YEY"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "PR6dlaI0lJLdjQdEwTqpn7f6xacDTU60"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include "AS726X.h"
AS726X sensor;
 
#include <Wire.h> ///
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Rashmi";
char pass[] = "12345678";

BlynkTimer timer;

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V0)
{
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();

  // Update state
  digitalWrite(2,value);
  Blynk.virtualWrite(V1, value);
}

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent()
{
//  // You can send any value at any time.
//  // Please don't send more that 10 values per second.
  sensor.takeMeasurementsWithBulb(); //This is a hard wait while all 18 channels are measured
  int nir=0;
  //Wire.begin();
  nir = sensor.getCalibratedW();
  int gluco;
  gluco = (.00009*nir*nir)+(0.2069*nir)+22.847;
  //nir = digitalRead(D21);
  Blynk.virtualWrite(V2, gluco);
}

void setup()
{ 
  // Debug console
  Serial.begin(115200);
  pinMode(2,OUTPUT);
  //pinMode(21,INPUT);
  
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  // You can also specify server:
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, IPAddress(192,168,1,100), 8080);
  Wire.begin();
  Serial.read(); //Throw away the user's button
  if(sensor.begin() == false)
  {
    Serial.println("Sensor does not appear to be connected. Please check wiring. Freezing...");
    while(1);
  }
 
  sensor.disableIndicator();
  //sensor.disableIndicator(); //Turn off the blue status LED
  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);
}

void loop()
{
  //sensor.takeMeasurementsWithBulb(); //This is a hard wait while all 18 channels are measured
  //int nir;
  //nir=(int)sensor.getCalibratedW();
  //Blynk.virtualWrite(V2, nir);
  //Wire.begin();
  
  Blynk.run();
  timer.run();
  // You can inject your own code or combine it with other sketches.
  
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
  //LED play start from here

  sensor.takeMeasurementsWithBulb(); //This is a hard wait while all 18 channels are measured
  int nir=0;
  //Wire.begin();
  nir = sensor.getCalibratedW();
  int gluco;
  gluco = (.00009*nir*nir)+(0.2069*nir)+22.847;
  
  // Set the brightness to 5 (0=dimmest 7=brightest)
  display.setBrightness(5);

  display.showNumberDec(gluco);     
  delay(2000);
  display.clear();
}
