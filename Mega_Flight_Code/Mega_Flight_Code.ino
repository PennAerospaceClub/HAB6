//Maximum Runtime ~50 days due to millis() resetting

//IMU
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_9DOF.h>
#include <Time.h>

//#include <Adafruit_10DOF.h> //10Dof
//#include <Adafruit_BMP085_U.h> //10Dof barometric/temperature)

//SD
#include <SD.h>
#include <SPI.h>

//Serial
#include <SoftwareSerial.h>

//IMU
String imuDataString = "";
Adafruit_9DOF                dof   = Adafruit_9DOF();
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);
Adafruit_LSM303_Mag_Unified   mag   = Adafruit_LSM303_Mag_Unified(30302);
Adafruit_L3GD20_Unified       gyro  = Adafruit_L3GD20_Unified(20);

//Adafruit_BMP085_Unified       bmp   = Adafruit_BMP085_Unified(18001); //10dof

//Position
//Boundary Box UPDATE DAY OF LAUNCH WITH MOST RECENT SIMULATION
long int minLat = 39000000;//xx°xx.xxxx' 40010950
long int maxLat = 41000000; //40337066
long int minLong = 74000000; //75462700
long int maxLong = 78000000; //77044366
long int maxWantedAlt = 31000; //Maximum wanted altitude (BDRY) 31000

//Initialize Location Data
long int lat = -1; 
long int longit = -1;
long int alt = -1; //altitude in meters
long int maxAlt = 0; //measures in meters

//GPS Utility
String field = "";
String slat = "";
String slongit = "";
String salt = "";
boolean gooddata = false;

//SD
int CSPIN = 53;

//Sanity
boolean sane = false;
boolean inBdry = false;
boolean falling = true;
boolean initSD = false;

//LED
const int LED_GREEN = 23;
const int LED_YELLOW = 25;
const int LED_RED = 27;
//Blinking lights
boolean onLED = false;

//Timing (second and 5 second writes)
unsigned long int nextWrite1 = 0;
unsigned long int nextWrite5 = 0;

//Nichrome
const int NICHROME_PIN = 31;
boolean nichromeStarted = false;
boolean nichromeFinished = false;
unsigned long int nichromeEndTime = 0;
int nichromeCounter = 0;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600); //GPS
  
  //LED
  pinMode(LED_GREEN, OUTPUT); digitalWrite(LED_GREEN, HIGH);
  pinMode(LED_YELLOW, OUTPUT); digitalWrite(LED_YELLOW, HIGH);
  pinMode(LED_RED, OUTPUT); digitalWrite(LED_RED, HIGH);

  //IMU
  initIMU();
  delay(1000);

  //Nichrome
  pinMode(NICHROME_PIN, OUTPUT);
  digitalWrite(NICHROME_PIN, LOW);

  //End Setup
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_YELLOW, LOW);

  nextWrite5 = millis() + 5000;
  nextWrite1 = millis() + 1000;
}

  unsigned long int gpsMaxSearchTime = millis() + 5000;

void loop() {
  while(!sane){
    sane = initSane();
      readGPS();
 //     Serial.print(lat); Serial.print(", "); Serial.print(longit); Serial.print(", "); Serial.println(alt); 
  }
  
  readGPS();
  Serial.print(lat); Serial.print(", "); Serial.print(longit); Serial.print(", "); Serial.println(alt); 

  runIMU();
  Serial.println(imuDataString);

  nichromeCheck();
  
  //Time Controlled: SD, Serial LED
  if(millis() >= nextWrite5){
    String dataString = dataLogging();
    dataSD(dataString);
    Serial.println(dataString);
    
    nextWrite5 = millis() + 5000;
  }

  if(millis() >= nextWrite1){
    if(onLED){
      digitalWrite(LED_GREEN, LOW);
      onLED = false;
    }
    else{
      digitalWrite(LED_GREEN, HIGH);
      onLED = true;
    }
    nextWrite1 = millis() + 1000;
  }
}


