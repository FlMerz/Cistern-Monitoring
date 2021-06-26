#include "CisternMonitoring_DisplayLibrary.h"
#include "CisternMonitoring_HallSensorLib.h"
#include "CisternMonitoring_LoraTransmitterLib.h"
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic
//Library for WIFI Manager
//#include <FS.h>  
//#include <ESP8266WiFi.h>          //ESP8266 Core WiFi Library (you most likely already have this in your sketch)
#include <DNSServer.h>            //Local DNS Server used for redirecting all requests to the configuration portal
//#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
#include <ArduinoJson.h>          //https://github.com/bblanchon/ArduinoJson
#include <HTTPClient.h>
#include <Arduino.h>
#ifdef ESP32
  #include <SPIFFS.h>
#endif
#include <SPI.h>


//###############Pin Setup NodeMCU32 - 32S ############################################
//Pinout Display (SPI Communication) ESP32
//#define TFT_MOSI 23 // SDA Pin 
//#define TFT_SCLK 18 //SCK Pin
//#define TFT_CS    32  // Chip select control pin CS
//#define TFT_DC    33  // Data Command control pin RS
//#define TFT_RST   -1  // Reset pin (could connect to RST pin)

//Pinout Hall Sensor using I2C pins
#define TRIG_PIN 22 // RX
#define ECHO_PIN 21 // TX

//Pins zur Programmsteuerung
#define RESET_PIN 34// Input Pin to trigger the reset of the controller
#define DISPLAY_ON_PIN 14 // pin to control whether the display should be enabled or not
//unsigned int DISPLAY_ON_INPUT_PIN = 35;//27; //Input pin to check, whether the display on button is pressed optinal 35

//Pins for LORA using SPI
//MOSI  23
//SCK  18
//MISO 19
#define NSS 5 // SS Pin
#define RESET 17 // RESET Pin


// Global Variables / Programm Settings for Display / Size of tank
#define yStartPositionCistern 40
#define xStartPositionCistern 0 // same like space to edge of display
#define SpaceToBottomOfDisplay 0
#define borderWidth 10

//Global variables for Hall Sensor Measurement
#define SENSORRANGE 400 // Maximum sensor range 
#define NUMOFMEASUREMENTS 3 // Defines, how many real measurements shuold be done within "one" measurement. 

//define your default values here, if there are different values in config.json, they are overwritten.
char maxVolumeInLitre[10]="";
char maxHeightinCM[5]="";
char offsetInCM[5] = "";
char measureFreqInSec[5] = "";
char ConnectionMode[8]="Online";

int maxVolumeInLitre_int;
int maxHeightinCM_int;
int offsetInCM_int;
int measureFreqInSec_int = 2;

//Wifi Manager varialbes
bool foundFSConfig = false;
String AP_Name_ConfigMode= "Configuration";
String AP_PW_ConfigMode = ""; //must be at least 8 digits long

//identifier if Wireless Lora Receiver is connected
bool UseWirelessSensor = false;
#define LoraFrequency 915E6L
#define syncWord 10


//Deepsleep Settings
#define uS_TO_S_FACTOR 1000000ULL  /* Conversion factor for micro seconds to seconds */

  WiFiManagerParameter text_CisternCredentials_header("<br><p><b>Abmessung der Zisterne:</b></p><br>");
  WiFiManagerParameter custom_maxVolumeInLitre("maxVolumeInLitre", "Gesamtvolumen der Zisterne (Liter)", "", 10," required type=\"number\" min=\"0\" max=\"999999999\" step=\"1\"");
  WiFiManagerParameter custom_maxHeightinCM("maxHeightinCM", "Höhe der Zisterne (cm)", "", 5, " required type=\"number\" min=\"0\" max=\"9999\"  step=\"1\"");
  WiFiManagerParameter text_MeasurementSettings_header("<br><p><b>Messeinstellungen:</b></p><br>");
  WiFiManagerParameter custom_offsetInCM("offsetInCM", "Messhöhe", "", 5, " required type=\"number\" min=\"0\" max=\"9999\" step=\"1\"");
  WiFiManagerParameter custom_measureFreqInSec("measureFreqInSec", "Messhäufigheit (in Sek)", "", 5, " required type=\"number\" min=\"1\"  max=\"9999\" step=\"1\"");


//Using CisternMonitoring Library -- inits automatically the display
CisternMonitoringDisplay cMDisplay(yStartPositionCistern, xStartPositionCistern, SpaceToBottomOfDisplay,borderWidth, AP_Name_ConfigMode, AP_PW_ConfigMode );

//Using CisternMonitoringLib
HallSensor HallSensor(TRIG_PIN,  ECHO_PIN, SENSORRANGE, NUMOFMEASUREMENTS);

//Using LoraReceiverTransmitterLib 
LoraTransmitter LoraReceiver(866E6 ,syncWord, NSS , RESET);

//Wifi Manager Object
 WiFiManager wifiManager;
void PinConfiguration(){
   //Pin configuration
 pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(RESET_PIN,INPUT);
  pinMode(DISPLAY_ON_PIN, OUTPUT);
//  pinMode(DISPLAY_ON_INPUT_PIN, INPUT_PULLUP);
  digitalWrite(DISPLAY_ON_PIN, LOW);
  
}
//callback notifying us of the need to save config
void saveConfigCallback () {
    Serial.println("Save Config Callback triggered");
  //Check if user has entered some value for ssid
  Serial.println("Check if SSID has been set to verify if its in online or offline mode");
  Serial.println("Entered SSID in ConfigPortal: " + wifiManager.getConfigPortalSSID());
   if (wifiManager._ssid == ""){
    Serial.println("No SSID has been selected -> go for offline mode");
    strcpy(ConnectionMode, "Offline");
  }else{
    Serial.println("SSID: "+wifiManager._ssid + " has been selected -> go for Online mode");
     strcpy(ConnectionMode, "Online");
  }

  //read updated parameters
 cMDisplay.DrawSavedConfigView();
  Serial.println("overwrite default values with entered values from config portal");
  strcpy(maxVolumeInLitre, custom_maxVolumeInLitre.getValue());
  strcpy(maxHeightinCM, custom_maxHeightinCM.getValue());
  strcpy(offsetInCM, custom_offsetInCM.getValue());
  strcpy(measureFreqInSec, custom_measureFreqInSec.getValue());

  saveConfigToFS();
}

void readConfigfromFS(){
Serial.println("Try to read config from FS");
//read configuration from FS json


  Serial.println("mounting FS...");

  if (SPIFFS.begin()) {
    Serial.println("mounted file system");
    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      Serial.println("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);
        if (json.success()) {
          Serial.println("\nparsed json");

          strcpy(maxVolumeInLitre, json["maxVolumeInLitre"]);
          strcpy(maxHeightinCM, json["maxHeightinCM"]);
          strcpy(offsetInCM, json["offsetInCM"]);
          strcpy(measureFreqInSec, json["measureFreqInSec"]);
          strcpy(ConnectionMode, json["connectionMode"]);
          
            Serial.println("the following have been read from Config File");
             Serial.println(maxVolumeInLitre);
             Serial.println(maxHeightinCM);
             Serial.println(offsetInCM);
             Serial.println(measureFreqInSec);
            Serial.println(ConnectionMode);
             
          if(maxVolumeInLitre !="" && maxHeightinCM !="" && offsetInCM!=""){
            //Check if there is already a config saved in FS
            foundFSConfig=true;
             Serial.println("Config File has been found.");
          }

        } else {
          Serial.println("failed to load json config");
        }
        configFile.close();
      }
    }
  } else {
    Serial.println("failed to mount FS");
    Serial.println("FS will be formatted.");
    SPIFFS.format();
  }
  //end read
  
}
void ConfigureDeepSleep(){
  esp_sleep_enable_timer_wakeup(measureFreqInSec_int * uS_TO_S_FACTOR);
  Serial.println("Setup ESP32 to sleep for every " + String(measureFreqInSec_int) +" Seconds");  
}
void saveConfigToFS(){
    Serial.println("saving config to FS");
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    json["maxVolumeInLitre"] = maxVolumeInLitre;
    json["maxHeightinCM"] = maxHeightinCM;
    json["offsetInCM"] = offsetInCM;
    json["measureFreqInSec"] = measureFreqInSec;
    json["connectionMode"] = ConnectionMode;
    
    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile) {
     Serial.println("failed to open config file for writing");
   }

    json.printTo(Serial);
    json.printTo(configFile);
    configFile.close();
    //end save
}

void CastValuesToINT(){
maxVolumeInLitre_int = atoi(maxVolumeInLitre);
maxHeightinCM_int = atoi(maxHeightinCM);
offsetInCM_int = atoi(offsetInCM);
measureFreqInSec_int = atoi(measureFreqInSec);
}


void ResetController(){  
digitalWrite(DISPLAY_ON_PIN, HIGH);
  Serial.println("Reset has been triggered. Controller will be reset");
    cMDisplay.DrawResetView();
    Serial.println("Reset Wifi Manger");
     wifiManager.resetSettings();
    Serial.println("Starting File System to remove file!");
 if (SPIFFS.begin()) {
    if (SPIFFS.exists("/config.json")) {
      //clear SPIFFS 
       Serial.println("Remove Config.json file from SPIFFS");
      SPIFFS.remove("/config.json");
    } 
  } else {
        SPIFFS.format();
    Serial.println("failed to mount FS");
  }
  
}
void startConfigMode(){
  Serial.println("Start Configuration Portal...");
  //here maybe a internet reset settings have to be inserted.
 Serial.println("Enable Display");
digitalWrite(DISPLAY_ON_PIN, HIGH);

//Reset all Wifimanager Settings
 wifiManager.resetSettings();
  cMDisplay.DrawConfigView();

  wifiManager.setSaveConfigCallback(saveConfigCallback);
  wifiManager.setBreakAfterConfig(true); // that save config will be called even if no internet has established
  //set custom ip for portal
  wifiManager.setAPStaticIPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));
  //add all your parameters here
    wifiManager.addParameter(&text_CisternCredentials_header);
  wifiManager.addParameter(&custom_maxVolumeInLitre);
  wifiManager.addParameter(&custom_maxHeightinCM);
  wifiManager.addParameter(&custom_offsetInCM);  
  wifiManager.addParameter(&text_MeasurementSettings_header);
  wifiManager.addParameter(&custom_measureFreqInSec);
  
     if (!wifiManager.autoConnect(AP_Name_ConfigMode.c_str(),AP_PW_ConfigMode.c_str())) {
      if(String(ConnectionMode)=="Online"){
         Serial.println("failed to connect in online modues. ");
         cMDisplay.DrawFailedOnlineModeConfigView();
        delay(3000);
        //reset and try again, or maybe put it to deep sleep
        Serial.println("ESP will be reset!");
  
          ESP.restart();
        delay(5000);
      }
     
    }
    if(String(ConnectionMode)=="Online"){
        //if you get here you have connected to the WiFi
      Serial.println("connected to WIFI");
      Serial.print("local ip");
      Serial.println(WiFi.localIP());
    }
}
void uploadDataToServer(int distance) {
    Serial.println("Prepare Server Upload...");
  HTTPClient http;  //Declare an object of class HTTPClient
   Serial.println("Check WIFI Status");
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    Serial.println("WIFI Status: CONNECTED!");
     Serial.println("Start Upload data");
    http.begin("http://nonsslapis.fammerz.de/cistern-monitoring/set_value.php?value=" + String(distance) + "&uniqueID=" + String(WiFi.macAddress())); //Specify request destination
    int httpCode = http.GET();  //Send the request
      Serial.println("http Response Code: " + httpCode );
    Serial.println("");
    if (httpCode > 0) { //Check the returning code
      Serial.println("Server Response: "+ http.getString());        //Print the response from webserver
    }
    http.end();   //Close connection
  }else{
 Serial.println("WIFI Status not Connected. Unable to Upload Data" );
  }
}

void onReceive(int packetSize) {
  // received a packet
  Serial.print("Received packet '");

  // read packet
  for (int i = 0; i < packetSize; i++) {
    Serial.print((char)LoRa.read());
  }

  // print RSSI of packet
  Serial.print("' with RSSI ");
  Serial.println(LoRa.packetRssi());
}

void setup(void) {

//start serial connection
Serial.begin(9600);

  while(!Serial) { }
  

  
//perfom pin configuration
PinConfiguration();

//Intilaizie Display
cMDisplay.initializeDisplay();

//Reset Section if Reset Button has pressed
Serial.println("Voltage of RESET PIN: " + String(digitalRead(RESET_PIN)));
if(digitalRead(RESET_PIN)== HIGH){
    ResetController();
 }

//Try to init Lora Sensor
UseWirelessSensor =  LoraReceiver.init();
Serial.println("Result of Lorainitialization: " + String(UseWirelessSensor));

//Try to read config from FS
readConfigfromFS();

 if(foundFSConfig==false ){
    Serial.println("No Config found -> start config mode");
    startConfigMode();
 }
  Serial.println("Connection Mode: " + String(ConnectionMode));
  if(String(ConnectionMode)=="Online"){
    //uses the autoconnect function of arduino to connect to the last network. 
 Serial.println("Wifi begin()");
 WiFi.begin();
  }

CastValuesToINT();
//Configure DeepSleep
ConfigureDeepSleep();

cMDisplay.setOffset(offsetInCM_int);

 //Draw Cistern on Diplay and load mainform
 cMDisplay.DrawMainView(UseWirelessSensor, ConnectionMode);

}

void handleNewMeasurement(int MeasuredDistance){
int Volume = -999;
int VolumeInPercentage =0;
int Distance = -999;

//Check if MeasuredDistance is realistiv value                                                                 
Serial.println("Check if measured value is realistic..");
Serial.println("Value to check ( Measured Distance): " + String(MeasuredDistance));

//reduce with offset
if(MeasuredDistance - offsetInCM_int>0){
Distance =  MeasuredDistance;
//calculate Volume
Volume =  maxVolumeInLitre_int -  ((Distance - offsetInCM_int)*maxVolumeInLitre_int)/maxHeightinCM_int;
//Calculate percentage
VolumeInPercentage = 100*Volume/ maxVolumeInLitre_int;
}

if(String(ConnectionMode)=="Online"){
  Serial.println("WifiSTatus: " + String(WiFi.isConnected()));
  if(WiFi.status() == WL_CONNECTED){
    uploadDataToServer(Distance-offsetInCM_int);
    cMDisplay.updateWaterLevel(VolumeInPercentage, Volume,Distance, ConnectionMode, true );
    //after uploading data to server go into deepsleep
    Serial.println("Start deepspleep!");
    esp_deep_sleep_start();
  }else{
      //Try it one more time to connect if it fails the first time
      Serial.println("First try to connect failed. Try to reconnect()");
      WiFi.reconnect();
      delay(3000);
      if(WiFi.status() == WL_CONNECTED){
        Serial.println("Second reconnect was successfully!");
        uploadDataToServer(Distance-offsetInCM_int);
        cMDisplay.updateWaterLevel(VolumeInPercentage, Volume,Distance,ConnectionMode,  true );
        esp_deep_sleep_start();
      }else{
        Serial.println("Second reconnect was not successfull!");
        //If no connection can be build up in the second try, write the distance out on the display but with offline symbol
        cMDisplay.updateWaterLevel(VolumeInPercentage, Volume,Distance,ConnectionMode, false);
        esp_deep_sleep_start();
      }
   
      }
}else{ // Offline Mode
   cMDisplay.updateWaterLevel(VolumeInPercentage, Volume,Distance,ConnectionMode, false );
    esp_deep_sleep_start();
}



//Disable display after first measurement
digitalWrite( DISPLAY_ON_PIN,LOW);


}
void loop() {
  int MeasuredDistance=-1;
 // Serial.println("Display On INput Pin State: " + String(digitalRead(DISPLAY_ON_INPUT_PIN)));
//measure Distance with Hall sensor

if(UseWirelessSensor){
  Serial.println("start waiting to receive lora data");
  String packet ="";
  while(MeasuredDistance<0){
  while(packet.length()==0){
    //Wait until lora receiver recieves some data... 
    packet = LoraReceiver.receivePacket();
     MeasuredDistance =  packet.toInt();
    }
  }
}else{
   Serial.println("use wired HallSensor to measure distance");
   MeasuredDistance = HallSensor.getDistance();
}

handleNewMeasurement(MeasuredDistance);


}
