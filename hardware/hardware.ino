//  ELET2415 WeatherStation Project_GL
//  Modified by yours truly
//  No I wont say my references
//  Just know I am him

// IMPORT ALL REQUIRED LIBRARIES
#include <rom/rtc.h>
#include <ArduinoJson.h>
#include <math.h>

#include <SPI.h>
#include <Adafruit_BMP280.h>
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "DHT.h"
#include <Wire.h>

#ifndef _WIFI_H 
#include <WiFi.h>
#include <HTTPClient.h>
#endif

#ifndef STDLIB_H
#include <stdlib.h>
#endif

#ifndef STDIO_H
#include <stdio.h>
#endif

#ifndef ARDUINO_H
#include <Arduino.h>
#endif

#ifndef ARDUINOJSON_H
#include <ArduinoJson.h>
#endif 
 
// DEFINE VARIABLES
#define ARDUINOJSON_USE_DOUBLE      1 

// DEFINE THE CONTROL PINS FOR THE DHT22 
#define DHTPIN 32 //datapin of sensor
#define DHTTYPE DHT22 // define the type of sensor
DHT dht(DHTPIN, DHTTYPE);

// define data pin and wet to dry values for Soil Moisture sensor
#define SOIL 25
const int DRY = 2700; //Analogue Reading when sensor is dry
const int WET = 1000; //Analogue reading when sensor is wet
int SoilMoistPerc = 0;

// define tft control and data pins
#define TFT_DC    17
#define TFT_CS    5
#define TFT_RST   16
#define TFT_CLK   18
#define TFT_MOSI  23
#define TFT_MISO  19


/* Initialize class objects*/
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);
Adafruit_BMP280 bmp; //I2C

// MQTT CLIENT CONFIG  
static const char* pubtopic      = "620146473";                    // Add your ID number here
static const char* subtopic[]    = {"620146473_sub","/elet2415"};  // Array of Topics(Strings) to subscribe to
static const char* mqtt_server   = "test.mosquitto.org";         // Broker IP address or Domain name as a String 

static uint16_t mqtt_port        = 1883;

// WIFI CREDENTIALS
// const char* ssid       = "Matthew's Galaxy A21s";     // Add your Wi-Fi ssid
// const char* password   = "yxoy_253==>8"; // Add your Wi-Fi password

const char* ssid = "MonaConnect";   // Add your Wi-Fi ssid
const char* password = "";  // Add your Wi-Fi password

// TASK HANDLES 
TaskHandle_t xMQTT_Connect          = NULL; 
TaskHandle_t xNTPHandle             = NULL;  
TaskHandle_t xLOOPHandle            = NULL;  
TaskHandle_t xUpdateHandle          = NULL;
TaskHandle_t xButtonCheckeHandle    = NULL;  

// FUNCTION DECLARATION   
void checkHEAP(const char* Name);   // RETURN REMAINING HEAP SIZE FOR A TASK
void initMQTT(void);                // CONFIG AND INITIALIZE MQTT PROTOCOL
unsigned long getTimeStamp(void);   // GET 10 DIGIT TIMESTAMP FOR CURRENT TIME
void callback(char* topic, byte* payload, unsigned int length);
void initialize(void);
bool publish(const char *topic, const char *payload); // PUBLISH MQTT MESSAGE(PAYLOAD) TO A TOPIC
void vButtonCheck( void * pvParameters );
void vUpdate( void * pvParameters );  
bool isNumber(double number);

//############### IMPORT HEADER FILES ##################
#ifndef NTP_H
#include "NTP.h"
#endif

#ifndef MQTT_H
#include "mqtt.h"
#endif



void setup() {
  Serial.begin(115200);

  // Initialize TFT LCD screen
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(ILI9341_BLUE);

  // Initialize DHT22 sensor
  dht.begin();
  
  //BMP CONFIG SHIT
  unsigned status;
  status = bmp.begin(0x76);
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
  
  
  initialize();            // INIT WIFI, MQTT & NTP
}



void loop() {
  // PRINT SENSOR VALUES TO SERIAL MONITOR

  SoilMoistPerc = map(analogRead(SOIL),DRY,WET,0,100);
  if (SoilMoistPerc >= 100) //if statement to account for errors/slight deviation in sensor readings
  {
    SoilMoistPerc = 100;
  }
  else if (SoilMoistPerc <= 0)
  {
    SoilMoistPerc = 0;
  }
  Serial.print(F("Soil Moisture Level: "));
  Serial.print(SoilMoistPerc);
  Serial.print(F("% "));

  Serial.print(F("Humidity: "));
  Serial.print(dht.readHumidity());

  Serial.print(F("DHTtemperature: "));
  Serial.print(dht.readTemperature());
  Serial.print(F("°C "));

  Serial.print(F("Heat index: "));
  Serial.print(dht.computeHeatIndex(dht.readTemperature(), dht.readHumidity(), false));
  Serial.print(F("°C "));

  Serial.print(F("BMPTemperature = "));
  Serial.print(bmp.readTemperature());
  Serial.println("°C ");

  Serial.print(F("Pressure = "));
  Serial.print(bmp.readPressure());
  Serial.println("Pa ");

  Serial.print(F("Approx altitude = "));
  Serial.print(bmp.readAltitude(1015)); /* Adjusted to local forecast! */
  Serial.println("m ");

  // DISPLAY SENSOR VALUES ON TFT LCD SCREEN
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLUE);
  tft.setTextSize(2);

  tft.print(" BASIC WEATHERSTATION_GL ");
  tft.println("");
  tft.println("");

  tft.print("DHT Temperature: ");
  tft.print(dht.readTemperature());
  tft.println(" *C");
  tft.println("");

  tft.print("BMP Temperature: ");
  tft.print(bmp.readTemperature());
  tft.println(" *C");
  tft.println("");

  tft.print("DHT Humidity: ");
  tft.print(dht.readHumidity());
  tft.println(" %");
  tft.println("");

  tft.print("Altitude: ");
  tft.print(bmp.readAltitude(1015));
  tft.println(" m");
  tft.println("");

  tft.print("Pressure: ");
  tft.print(bmp.readPressure());
  tft.println(" Pa");
  tft.println("");

  tft.print("Soil Moisture Level: ");
  tft.print(int(SoilMoistPerc));
  tft.println(" %");
  tft.println("");

  tft.print("Heat index: ");
  tft.print(dht.computeHeatIndex(dht.readTemperature(), dht.readHumidity(), false));
  tft.print(" *C");

  Serial.println();
    delay(1000);

}

//####################################################################
//#                          UTIL FUNCTIONS                          #       
//####################################################################
void vButtonCheck( void * pvParameters )  {
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );     
      
    for( ;; ) {
        // Add code here to check if a button(S) is pressed
        // then execute appropriate function if a button is pressed  
        
        vTaskDelay(200 / portTICK_PERIOD_MS);  
    }
}

void vUpdate( void * pvParameters )  {
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );    
           
    for( ;; ) {
          // #######################################################
          // ## This function must PUBLISH to topic every second. ##
          // #######################################################

              // 1. Create JSon object
              StaticJsonDocument<1000> doc;
              
              // 2. Create message buffer/array to store serialized JSON object
              char message[1100]  = {0};
              
              // 3. Add key:value pairs to JSon object based on above schema
              doc["id"]                 = "620146473";
              doc["timestamp"]          = getTimeStamp();
              doc["dhtTemperature"]     = dht.readTemperature();
              doc["bmpTemperature"]     = bmp.readTemperature();
              doc["humidity"]           = dht.readHumidity();
              doc["altitude"]           = bmp.readAltitude(1015);
              doc["pressure"]           = bmp.readPressure();
              doc["soilmoisturelevel"]  = int(SoilMoistPerc);
              doc["heatindex"]          = dht.computeHeatIndex(dht.readTemperature(), dht.readHumidity(), false);

              // 4. Seralize / Covert JSon object to JSon string and store in message array
              serializeJson(doc, message);
               
              // 5. Publish message to a topic sobscribed to by both backend and frontend
              if (mqtt.connected()) {
                publish(pubtopic, message);
              }
          }
    vTaskDelay(1000 / portTICK_PERIOD_MS);
}               

unsigned long getTimeStamp(void) {
          // RETURNS 10 DIGIT TIMESTAMP REPRESENTING CURRENT TIME
          time_t now;         
          time(&now); // Retrieve time[Timestamp] from system and save to &now variable
          return now;
}

void callback(char* topic, byte* payload, unsigned int length) {
  // ############## MQTT CALLBACK  ######################################
  // RUNS WHENEVER A MESSAGE IS RECEIVED ON A TOPIC SUBSCRIBED TO

  Serial.printf("\nMessage received : ( topic: %s ) \n", topic);
  char* received = new char[length + 1]{ 0 };

  for (int i = 0; i < length; i++) {
    received[i] = (char)payload[i];
  }

  // PRINT RECEIVED MESSAGE
  Serial.printf("Payload : %s \n", received);


  // CONVERT MESSAGE TO JSON


  // PROCESS MESSAGE
}

bool publish(const char *topic, const char *payload){   
     bool res = false;
     try{
        res = mqtt.publish(topic,payload);
        // Serial.printf("\nres : %d\n",res);
        if(!res){
          res = false;
          throw false;
        }
     }
     catch(...){
      Serial.printf("\nError (%d) >> Unable to publish message\n", res);
     }
  return res;
}


