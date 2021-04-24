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
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <BH1750.h>


// Replace with your network credentials
const char* ssid     = "YOUR_WLAN_NAME";
const char* password = "YOUR_WLAN_PASSWORD";

// REPLACE with your Domain name and URL path or IP address with path
const char* serverName = "http://192.168.0.110/post-esp-data.php";

// Keep this API Key value to be compatible with the PHP code provided in the project page. 
// If you change the apiKeyValue value, the PHP file /post-esp-data.php also needs to have the same key 
String apiKeyValue = "tPmAT5Ab3j7F9";

String sensorLocation = "Office";
int requestFrequency = 30000; //ms

/*#include <SPI.h>
#define BME_SCK 18
#define BME_MISO 19
#define BME_MOSI 23
#define BME_CS 5*/

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BMP280 bmp;  // I2C
BH1750 lightMeter;

//Adafruit_BMP280 bme(BME_CS);  // hardware SPI
//Adafruit_BMP280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK);  // software SPI

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

  // BMP280
  // (you can also pass in a Wire library object like &Wire2)
  Serial.println("START BMP280");
  bool statusBMP = bmp.begin(0x76);
  if (!statusBMP) {
    Serial.println("Could not find a valid BME280 sensor, check wiring or change I2C address!");
    while (1);
  }

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
  
}

void loop() {
  httpRequest("Temperature");
  httpRequest("Pressure");
  httpRequest("Light");
  
 //Send an HTTP POST request every 30 seconds
  delay(requestFrequency);  
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

    // Choose Sensor
    if (meassurement == "Temperature") {
      Serial.println("httpRequest for: " + meassurement);
      httpRequestData = httpRequestData + "&value=" + String(bmp.readTemperature()) + "&unit=C";
      }
    if (meassurement == "Pressure") {
      Serial.println("httpRequest for: " + meassurement);
      httpRequestData = httpRequestData + "&value=" + String(bmp.readPressure()) + "&unit=Pa";
      }
    if (meassurement == "Light") {
      Serial.println("httpRequest for: " + meassurement);
      httpRequestData = httpRequestData + "&value=" + String(lightMeter.readLightLevel()) + "&unit=lx";
      }

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
