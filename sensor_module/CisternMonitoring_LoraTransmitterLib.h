/*
  CisternMonitoring_HallSensorLib - Library for HallSensor Control for Cistern Monitoring.
  Created by Florian Merz, 21.11.2020
  Just for internal purpose
*/
#ifndef CISTERNMONITORING_LORATRANSMITTERLIB_H
#define CISTERNMONITORING_LORATRANSMITTERLIB_H

#include <Arduino.h>
#include <LoRa.h>


class LoraTransmitter
{
  public:
         LoraTransmitter( long frequency, int syncWord, unsigned int NSS, unsigned int RESET);
    bool init();
     String sendPacket(int message);
  private:
    unsigned int _NSS; // SS Pin
    unsigned int _RESET; // RESET Pin
    long _frequency;
    int _syncWord;
};
#endif
