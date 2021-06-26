/*
  CisternMonitoring_HallSensorLib - Library for HallSensor Control for Cistern Monitoring.
  Created by Florian Merz, 21.11.2020
  Just for internal purpose
*/
#ifndef CISTERNMONITORING_HALLSENSORLIB_H
#define CISTERNMONITORING_HALLSENSORLIB_H

#include <Arduino.h>
#include <RunningMedian.h>

class HallSensor
{
  public:
         HallSensor(unsigned int TRIG_PIN, unsigned int ECHO_PIN, unsigned int SENSORRANGE, unsigned int NUMOFMEASUREMENTS);
    int getDistance();

  private:
      unsigned int _deviationFactor;
      unsigned int _TRIG_PIN;
      unsigned int _ECHO_PIN;
      unsigned int _SENSORRANGE;
      unsigned int _NUMOFMEASUREMENTS;
      RunningMedian MedianFilter;
};
#endif
