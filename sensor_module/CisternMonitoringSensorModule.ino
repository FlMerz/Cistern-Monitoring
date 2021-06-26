#include "LowPower.h"
#include "CisternMonitoring_HallSensorLib.h"
#include "CisternMonitoring_LoraTransmitterLib.h"

//############################Pin Declaration - Start################################
//Pin setup LORA Sensor
//Pins for LORA using SPI
//MOSI  23
//SCK  18
//MISO 19
#define LORA_SYNCWORD    10  // SyncWord for LORA
#define LORA_NSS         10  //SS Pin
#define LORA_RESET       0  //Reset Pin (A0)
#define LORA_FREQ        866E6  //LORA Freq
#define LORA_ENABLE_PIN   17

//Hall Sensor
#define HALL_TRIG_PIN        19   //RX   A5
#define HALL_ECHO_PIN        18   //TX   A4  
#define HALL_ENABLE_PIN      5
#define HALL_MAX_DISTANCE    300  //Maximum Distance for Sensor

//DeeplSleep
#define wakeUpPin 2  
//############################Pin Declaration - End################################




//Global variables for Hall Sensor Measurement
#define NUMOFMEASUREMENTS 3 // Defines, how many real measurements shuold be done within "one" measurement. 

//Using CisternMonitoringLib
HallSensor HallSensor(HALL_TRIG_PIN,  HALL_ECHO_PIN, HALL_MAX_DISTANCE, NUMOFMEASUREMENTS);

//Using LoraReceiverTransmitterLib 
LoraTransmitter LoraSender(LORA_FREQ , LORA_SYNCWORD, LORA_NSS , LORA_RESET);
int MeasuredDistance;

void wakeUp()
{
    // Just a handler for the pin interrupt.
}
void setup() {

  // initialize digital pin LED_BUILTIN as an output.
    Serial.begin(9600);

  //init lora sender
  LoraSender.init();  
 // pinMode(wakeUpPin, INPUT);  
  //
  //pinMode(LORA_ENABLE_PIN, OUTPUT);
  //pinMode(HALL_ENABLE_PIN, OUTPUT);
  
  
}

void loop() {

    // Allow wake up pin to trigger interrupt on low.
  //  attachInterrupt(0, wakeUp, HIGH);

    
   Serial.println("SET HALL ENABLE PIN TO HIGH");
  // digitalWrite(HALL_ENABLE_PIN, HIGH);
  //  digitalWrite(LORA_ENABLE_PIN, LOW);
        Serial.println("ENable power down");
  //LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF );

 // detachInterrupt(0); 
    Serial.println("Woke up");
    Serial.println("Enable Hall Sensor");
     //digitalWrite(HALL_ENABLE_PIN, LOW);
     //digitalWrite(LORA_ENABLE_PIN, HIGH);
     delay(1000);
    MeasuredDistance = HallSensor.getDistance();
      Serial.println("Send Packet via LORA");
    LoraSender.sendPacket(MeasuredDistance);    
    // digitalWrite(HALL_ENABLE_PIN, HIGH);
    // digitalWrite(LORA_ENABLE_PIN, LOW);
}
