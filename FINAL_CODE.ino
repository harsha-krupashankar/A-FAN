#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <HTTPClient.h>
#include "DHT.h"
#include <Wire.h>
#include <SPI.h>
#include <ACROBOTIC_SSD1306.h>

// Defining DHT sensor variables and objects
#define DHTPIN 4     // Digital pin 4 connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);

BlynkTimer timer;     // Defining "timer" object of "BlynkTimer" class

// These are the pins of the motor driver which are used to control the fan speed and direction
int motor1Pin3 = 35;      
int motor1Pin4 = 32; 
int enable1Pin = 14; 

// PWM Properties
const int freq = 5000;
const int pwmChannel = 0;
const int resolution = 10;
int dutyCycle = 0;             // Initializing the duty cycle value to 0
int fanSpeedPercent = 0;       // Initializing the fan speed percent to 0%


// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "kTB9RXVBW72rwkjdY6fSow5iZATejcru";

const char* ssid = "Realme U1";           // Our Wifi name  
const char* password = "12345678";        // Our wifi password

String serverName = "https://api.thingspeak.com/update?api_key=9YNTT4KW3AIL4615";       // The URL of our ThingSpeak channel

unsigned long lastTime = 0;
unsigned long timerDelay = 60000;

float x = 0; //Temporary Variable

void setup() {

  pinMode(motor1Pin3, OUTPUT);
  pinMode(motor1Pin4, OUTPUT);
  pinMode(enable1Pin, OUTPUT);
  
  // configure PWM functionalitites
  ledcSetup(pwmChannel, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(enable1Pin, pwmChannel);

  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));
  Wire.begin();  
  oled.init();                      // Initialze SSD1306 OLED display
  oled.clearDisplay();

  Blynk.begin(auth, ssid, password);      // Connects the esp32 to Blynk cloud if every credential is valid and matched
  dht.begin();
  timer.setInterval(25000L, sendSensor);    // Execute the main logic code for every 2.5 seconds 

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  Blynk.run();
  timer.run();
}

void sendSensor() {
  
  // Read the humidity value
  float hmdt = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float temp = dht.readTemperature();
  

  // Check if any reads failed and exit early (to try again).
  if (isnan(hmdt) || isnan(temp)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  Serial.print(F("Humidity: "));
  Serial.print(hmdt);
  
  Serial.print(F("Temperature: "));
  Serial.print(temp);

  oled.setTextXY(1,0);// Clear screen
  oled.putString("Humidity:");
  oled.setTextXY(1,10);
  oled.putNumber(hmdt);

  
  oled.setTextXY(3,0);// Clear screen
  oled.putString("Temperature:");
  oled.setTextXY(3,13);
  oled.putNumber(temp); 


  digitalWrite(enable1Pin, HIGH);
  
  //Move DC motor forward with increasing speed
  digitalWrite(motor1Pin3, LOW);
  digitalWrite(motor1Pin4, HIGH);

  // if temperature is less than 27 degree celsius, turn off the fan
  if(temp<27) {
    digitalWrite(enable1Pin, LOW);      // Turning off the fan
    delay(1000);  
    oled.setTextXY(5,0);// Clear screen
    oled.putString("Fan Speed:");
    oled.setTextXY(5,11);
    oled.putNumber(fanSpeedPercent);
  }

  // If the temperature is between 27 and 30 degree celsius, then fan will run at 20% speed
  if(temp>=27 && temp<30) {
    dutyCycle = 520;
    fanSpeedPercent = 20;

    ledcWrite(pwmChannel, dutyCycle);        // setting the voltage for the fan
    Serial.println(" The fan is rotating at " + String(fanSpeedPercent) + "%");  
    
    delay(1000);
    oled.setTextXY(5,0);// Clear screen
    oled.putString("Fan Speed:");
    oled.setTextXY(5,11);
    oled.putNumber(fanSpeedPercent);
    
  }

  // If the temperature is between 30 and 33 degree celsius, then fan will run at 40% speed
  if(temp>=30 && temp<33) {
    dutyCycle = 640;
    fanSpeedPercent = 40;
    ledcWrite(pwmChannel, dutyCycle);       // setting the voltage for the fan
    Serial.println(" The fan is rotating at " + String(fanSpeedPercent) + "%");
    
    delay(1000);
    oled.setTextXY(5,0);// Clear screen
    oled.putString("Fan Speed:");
    oled.setTextXY(5,11);
    oled.putNumber(fanSpeedPercent);
   
  }

  // If the temperature is between 33 and 36 degree celsius, then fan will run at 60% speed
  if(temp>=33 && temp<36) {
    dutyCycle = 760;
    fanSpeedPercent = 60;
    ledcWrite(pwmChannel, dutyCycle);         // setting the voltage for the fan
    Serial.println(" The fan is rotating at " + String(fanSpeedPercent) + "%");
   
    delay(1000);
    oled.setTextXY(5,0);// Clear screen
    oled.putString("Fan Speed:");
    oled.setTextXY(5,11);
    oled.putNumber(fanSpeedPercent);
    
  }

  // If the temperature is between 36 and 39 degree celsius, then fan will run at 80% speed
  if(temp>=36 && temp<39) {
    dutyCycle = 880;
    fanSpeedPercent = 80;
    ledcWrite(pwmChannel, dutyCycle);         // setting the voltage for the fan
    Serial.println(" The fan is rotating at " + String(fanSpeedPercent) + "%");
   
    delay(1000);
    oled.setTextXY(5,0);// Clear screen
    oled.putString("Fan Speed:");
    oled.setTextXY(5,11);
    oled.putNumber(fanSpeedPercent);
    
  }

  // If the temperature is greater than 39 degree celsius , then fan will run at 100% speed
  if(temp>=39) {
    dutyCycle = 1023;
    fanSpeedPercent = 100;
    ledcWrite(pwmChannel, dutyCycle);       // setting the voltage for the fan
    Serial.println(" The fan is rotating at " + String(fanSpeedPercent) + "%");
   
    delay(1000);
    oled.setTextXY(5,0);// Clear screen
    oled.putString("Fan Speed:");
    oled.setTextXY(5,11);
    oled.putNumber(fanSpeedPercent);
   
  }

  //Writing the temperature and fan speed percent values to virtual pins v6 and v7 in Blynk app
  Blynk.virtualWrite(V6, temp);
  Blynk.virtualWrite(V7, fanSpeedPercent);

  
  Serial.println("ESP32 Alert - Temperature = " + String(temp) + " °C," + " Fan Speed = " + String(fanSpeedPercent)+ "%");
  //Blynk.email("harshahasadyant99@gmail.com", "ESP8266 Alert", "Temperature over 28C!");

  // If the temperature changes to one or more than one degree celsius, then only send the notifications to mobile
  if(int(x) != int(temp)) {
      Blynk.notify("ESP32 Alert - Temperature = " + String(temp) + " °C," + " Fan Speed = " + String(fanSpeedPercent)+ "%" );    // I have a doubt regarding this
  }

  x = temp;
  Serial.println(x);

  // Send the data to the cloud if the last time from sending the last data point is more than 1 minute
  if ((millis() - lastTime) > timerDelay) {
    
    // if our esp32 microcontroller is connected to the wifi
    if(WiFi.status()== WL_CONNECTED){
    // Read data from our dht object and pass it to our sendData function 
        sendData(temp, fanSpeedPercent, hmdt);
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    
   lastTime = millis();
  }  
}

// Function for sending the data to the cloud "ThingSpeak" and visualize that data in the form of graphs
void sendData(double temp, int fanSpeedPercent, double hmdt){
    HTTPClient http;    // Initializing the object "http" of "HTTPClient" class


    String url = serverName + "&field1=" + temp + "&field2=" + fanSpeedPercent + "&field3=" + hmdt ;  // The address of our thingspeak private channel
    
    http.begin(url.c_str());      // Your Domain name with URL path or IP address with path
      
    int httpResponseCode = http.GET();      // Send HTTP GET request

    // If it is a valid URL and our http request is sent successfully, we get an http response code which is always greater than 1
    if (httpResponseCode > 0){
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }else{
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    http.end();
}
