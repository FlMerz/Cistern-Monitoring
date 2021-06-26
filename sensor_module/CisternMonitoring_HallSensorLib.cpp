#include "CisternMonitoring_HallSensorLib.h"
#include <RunningMedian.h>


//Constructor
HallSensor::HallSensor(unsigned int TRIG_PIN, unsigned int ECHO_PIN, unsigned int SENSORRANGE, unsigned int NUMOFMEASUREMENTS):
MedianFilter(NUMOFMEASUREMENTS)
{

       _TRIG_PIN  = TRIG_PIN;
       _ECHO_PIN = ECHO_PIN;
       _SENSORRANGE = SENSORRANGE;
       _NUMOFMEASUREMENTS = NUMOFMEASUREMENTS;

  pinMode(_TRIG_PIN, OUTPUT);
  pinMode(_ECHO_PIN, INPUT);
}

int HallSensor::getDistance(){
  
  for (int i=1; i <= _NUMOFMEASUREMENTS; i++){
  digitalWrite(_TRIG_PIN, LOW);
  delayMicroseconds(2);// delayMicroseconds(2);
  digitalWrite(_TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(_TRIG_PIN, LOW);
   long duration = pulseIn(_ECHO_PIN, HIGH);
  int distance = duration/29/2;
   Serial.print("Measured Distance(raw): ");
   Serial.println(distance);
  MedianFilter.add(distance);
  delay(50);
  }
  int distance_medianfiltered = (int)MedianFilter.getMedian();
  Serial.print("MeasuredDistance (median filtered): ");
  Serial.println(distance_medianfiltered);
  MedianFilter.clear();
  return distance_medianfiltered;

}
