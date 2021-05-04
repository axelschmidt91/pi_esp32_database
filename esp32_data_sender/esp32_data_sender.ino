/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-esp8266-mysql-database-php/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

*/

#ifdef ESP32
  #include <WiFi.h>
  #include <HTTPClient.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>
  #include <WiFiClient.h>
#endif

#include <Wire.h>
#include "credentials.h"

// Replace with your network credentials
const char* ssid     = WIFI_SSID;
const char* password = WIFI_PW;

// REPLACE with your Domain name and URL path or IP address with path
const char* serverName = "http://192.168.0.110/post-esp-data.php";

// Keep this API Key value to be compatible with the PHP code provided in the project page. 
// If you change the apiKeyValue value, the PHP file /post-esp-data.php also needs to have the same key 
String apiKeyValue = API_KEY;

String sensorLocation = "Wohnzimmer";
int period = 10000; //ms

//#define DEBUG

// Used Sensors
//#define BH1750_SENSOR // Light Intensity
#define BMP280_SENSOR // Temperatur, Pressure
//#define KY038_SENSOR // Sound
#define CCS811_SENSOR // eCO2, VOC


 // Initation 
unsigned long time_now = 0;

#ifdef BH1750_SENSOR 
  
  #include <BH1750.h>
  BH1750 lightMeter;

#endif

#ifdef BMP280_SENSOR 

  #include <Adafruit_Sensor.h>
  #include <Adafruit_BMP280.h>
  #define BMP280_ADDR 0x76
  
  /*#include <SPI.h>
  #define BME_SCK 18
  #define BME_MISO 19
  #define BME_MOSI 23
  #define BME_CS 5*/
  
  #define SEALEVELPRESSURE_HPA (1013.25)
  
  Adafruit_BMP280 bmp;  // I2C
  
  //Adafruit_BMP280 bme(BME_CS);  // hardware SPI
  //Adafruit_BMP280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK);  // software SPI
  
#endif

#ifdef KY038_SENSOR 

  // Volume 
  const int sound_digital = 4;
  int volume_exceed_count = 0;

#endif

#ifdef CCS811_SENSOR

  // Connect WAKE to GND!

  #include "Adafruit_CCS811.h"
  Adafruit_CCS811 ccs;
  
  int waittime_sensor_warm_up =20*60*1000; // 20 min in ms; wait to warm up
  bool write_data = false;
  
#endif


void setup() {
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());


  #ifdef BMP280_SENSOR
    // BMP280
    // (you can also pass in a Wire library object like &Wire2)
    Serial.println("START BMP280");
    bool statusBMP = bmp.begin(BMP280_ADDR);
    if (!statusBMP) {
      Serial.println("Could not find a valid BMP280 sensor, check wiring or change I2C address!");
      while (1);
    }
  #endif

  #ifdef BH1750_SENSOR
    // BH1750
    // Initialize the I2C bus (BH1750 library doesn't do this automatically)
    Wire.begin();
    // On esp8266 you can select SCL and SDA pins using Wire.begin(D4, D3);
    // For Wemos / Lolin D1 Mini Pro and the Ambient Light shield use Wire.begin(D2, D1);
    Serial.println("START BH1750");
    bool statusBH1750 = lightMeter.begin();
    if (!statusBH1750) {
      Serial.println("Could not find a valid BH1750 sensor, check wiring or change I2C address!");
      while (1);
    }
  #endif

  #ifdef KY038_SENSOR
    Serial.println("START KY-038");
    pinMode(sound_digital, INPUT);
  #endif

  #ifdef CCS811_SENSOR

    Serial.println("START CCS811");

    if(!ccs.begin()){
    Serial.println("Failed to start sensor! Please check your wiring.");
    while(1);
    }
     
    //calibrate temperature sensor
    while(!ccs.available());
    float temp = ccs.calculateTemperature();
    ccs.setTempOffset(temp - 25.0);
    // ccs.setDriveMode(uint8_t 1); // mode 1: every 1s; mode 2: every 10s
  
  #endif
  
}

void loop() {

  #ifdef KY038_SENSOR
    volume();
  #endif
  
  if(abs(millis() - time_now) >= period){
    time_now += period;
    #ifdef BMP280_SENSOR
      httpRequest("Temperature");
      httpRequest("Pressure");
    #endif
    #ifdef BH1750_SENSOR
      httpRequest("Light");
    #endif
    #ifdef KY038_SENSOR
      httpRequest("Volume");
    #endif
    #ifdef CCS811_SENSOR
      if (ccs.available()) {
          if (!ccs.readData()) {
            httpRequest("eCO2");
            httpRequest("TVOC");
          }}
    #endif
    
    } 
}


void httpRequest(String meassurement) {
  //Check WiFi connection status
  if(WiFi.status()== WL_CONNECTED){
    HTTPClient http;
    
    // Your Domain name with URL path or IP address with path
    http.begin(serverName);
    
    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    // Prepare your HTTP POST request data
    String httpRequestData = "api_key=" + apiKeyValue + "&sensor=" + meassurement + "&location=" + sensorLocation;
    Serial.println("httpRequest for: " + meassurement);
    
    // Choose Sensor
    #ifdef BMP280_SENSOR
      if (meassurement == "Temperature") {
        httpRequestData = httpRequestData + "&value=" + String(bmp.readTemperature()) + "&unit=C";
        }
      if (meassurement == "Pressure") {
        httpRequestData = httpRequestData + "&value=" + String(bmp.readPressure()) + "&unit=Pa";
        }
    #endif
    #ifdef BH1750_SENSOR
      if (meassurement == "Light") {
        httpRequestData = httpRequestData + "&value=" + String(lightMeter.readLightLevel()) + "&unit=lx";
        }
    #endif
    #ifdef KY038_SENSOR
      if (meassurement == "Volume") {
        httpRequestData = httpRequestData + "&value=" + String(volume_exceed_count) + "&unit=#";
        volume_exceed_count = 0;
        }
    #endif
    #ifdef CCS811_SENSOR
      if (!write_data) {
        if(millis() >= waittime_sensor_warm_up) {
          write_data=true;
          }
        }
      if (meassurement == "eCO2") {
        httpRequestData = httpRequestData + "&value=" + String(ccs.geteCO2()) + "&unit=ppm";
        }
      if (meassurement == "TVOC") {
        httpRequestData = httpRequestData + "&value=" + String(ccs.getTVOC()) + "&unit=ppm";
        }
        
      if (!write_data) {
        Serial.print("httpRequestData: ");
        Serial.println(httpRequestData);
        Serial.println("waited time: " + String(float(millis())/1000/60, 2) + " min of " + String(float(waittime_sensor_warm_up)/1000/60, 2) + " min. write_data=" + String(write_data));
        httpRequestData="";
      }
    #endif
    
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);

    
    // You can comment the httpRequestData variable above
    // then, use the httpRequestData variable below (for testing purposes without the BME280 sensor)
    //String httpRequestData = "api_key=tPmAT5Ab3j7F9&sensor=BME280&location=Office&value1=24.75&value2=49.54&value3=1005.14";

    // Send HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);
     
    // If you need an HTTP request with a content type: text/plain
    //http.addHeader("Content-Type", "text/plain");
    //int httpResponseCode = http.POST("Hello, World!");
    
    // If you need an HTTP request with a content type: application/json, use the following:
    //http.addHeader("Content-Type", "application/json");
    //int httpResponseCode = http.POST("{\"value1\":\"19\",\"value2\":\"67\",\"value3\":\"78\"}");
        
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
}

#ifdef KY038_SENSOR
  void volume() {
    if (digitalRead(sound_digital) == HIGH) {
      volume_exceed_count++;
      }
    #ifdef DEBUG
      Serial.println("count: " + String(volume_exceed_count));
    #endif
    }
#endif
