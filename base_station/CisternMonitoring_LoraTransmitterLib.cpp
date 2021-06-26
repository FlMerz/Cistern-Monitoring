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
  LoRa.setPins(_NSS, _RESET, 25);
 
  if (!LoRa.begin(_frequency)) {
    Serial.println("Lora Initialization failed!");
    return false;
  }else{
     //LoRa.setSyncWord(_syncWord);
     Serial.println("Lora Initialization successfully!");
    return true;
  }
}

String LoraTransmitter::receivePacket(){
 // try to parse packet
  int packetSize = LoRa.parsePacket();
  String packet="";
  if (packetSize) {
    // read packet
    while (LoRa.available()) {
     packet += (char)LoRa.read();
    
    }
    Serial.println("LORA: Received Packet: " + packet);
  }
   
    return packet;
}
