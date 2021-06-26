#include "CisternMonitoring_LoraTransmitterLib.h"


//Constructor
LoraTransmitter::LoraTransmitter(long frequency, int syncWord, unsigned int NSS, unsigned int RESET)
{
    _frequency = frequency;
    _syncWord = syncWord;
    _NSS = NSS;
    _RESET = RESET;

    

}

bool LoraTransmitter::init(){
  Serial.println("starting initialization of LoRa Receiver");
  //LoRa.setPins(ss, reset, dio0)
  LoRa.setPins(_NSS, _RESET, 0);
  if (!LoRa.begin(_frequency)) {
    Serial.println("Lora Initialization failed!");
    return false;
  }else{
    //  LoRa.setSyncWord(_syncWord);
     Serial.println("Lora Initialization successfully!");
    return true;
  }
}


String LoraTransmitter::sendPacket(int message){
 Serial.print("Sending packet: ");
  Serial.println(message);
  // send packet
  LoRa.beginPacket();
  LoRa.print(message);
  LoRa.endPacket();
  Serial.println("Sending completed!");
}
