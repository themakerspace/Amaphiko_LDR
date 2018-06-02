#include <Adafruit_Sensor.h>

#include "DHT.h"
#define DHTTYPE DHT11 
#define DHTPIN 5     // what digital pin we're connected to
// ESP8266 boards installed via the boards manager but GIT is here - https://github.com/esp8266/Arduino

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// WiFi library from https://github.com/esp8266/Arduino but install via the libraries manager
DHT dht(DHTPIN, DHTTYPE);
int red = 15;
int green = 12;
int blue = 13;
int LDRPin = A0;


const char* ssid = "makers";
const char* password = "makelove";

boolean onOff = 0;

void setup() {
   Serial.begin(115200);
   Serial.print("Started...");
   pinMode(red, OUTPUT);
   pinMode(blue, OUTPUT);
   pinMode(green, OUTPUT);
   dht.begin();
   connectWifi();
}

void loop() {
/* float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");

  } else {

    // Compute heat index in Fahrenheit (the default)
    float hif = dht.computeHeatIndex(f, h);
    // Compute heat index in Celsius (isFahreheit = false)
    float hic = dht.computeHeatIndex(t, h, false);

    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print(" *C ");
    Serial.print(f);
    Serial.print(" *F\t");
    Serial.print("Heat index: ");
    Serial.print(hic);
    Serial.print(" *C ");
    Serial.print(hif);
    Serial.println(" *F");

  */
  ledCall("blue", 150);
  int LDRReading = 0;
  LDRReading = analogRead(LDRPin);
  Serial.println(LDRReading);
  sendGet("Amaphiko1","LDR1",LDRReading);
  //sendGet("MS_light1","TC102",h);
  //sendGet("MS_light1","TC103",t);
  delay(10000);
  
}


void sendGet(String sensor_id , String measurement_id , float value)
{
  ///domains/{domain}/sensors/{sensor_id}/measurements/{measurement_id}/values 
   HTTPClient http;  //Declare an object of class HTTPClient
   String Request = "http://dev.waziup.io:800/api/v1/domains/waziup/sensors/"+sensor_id+"/measurements/"+measurement_id+"/values";
    Serial.println(Request );
   http.begin(Request);  //Specify request destination
   http.addHeader("Content-Type", "application/json");
    Serial.println(sensor_id );
    Serial.println(measurement_id);
   http.addHeader("sensor_id", sensor_id );
   http.addHeader("measurement_id", measurement_id);

    
   String strStart = "{\"value\":\"";
  
   strStart.concat(value);
   String strEnd = strStart+"\",\"timestamp\": \"2018-06-02T13:56:27.873Z\"}" ;
   int httpCode = http.POST(strEnd);   //Send the request
   String payload = http.getString();                  //Get the response payload
   
   Serial.println("API call");
   Serial.println(strEnd);
   Serial.println("Response");
   Serial.println(httpCode);   //Print HTTP return code
   Serial.println(payload);    //Print request response payload
   http.end();   //Close connection
}

void ledCall(String colour, int strength){
  analogWrite(red, 0);
  analogWrite(green, 0);
  analogWrite(blue, 0);
  if(onOff == true){
  if(colour == "red"){
   analogWrite(red, strength); 
  }

  if(colour == "green"){
   analogWrite(green, strength);  
  }

  if(colour == "blue"){
   analogWrite(blue, strength); 
  }
  }
}
void connectWifi() {
  WiFi.begin(ssid, password);
  // put your setup code here, to run once:
  while (WiFi.status() != WL_CONNECTED) {
    ledCall("red", 150);
    delay(500);
    Serial.print(".");
  }
 Serial.println("");
 Serial.print("Connected to ");
 Serial.println(ssid);
 Serial.print("IP address: ");
 Serial.println(WiFi.localIP());
 
}

