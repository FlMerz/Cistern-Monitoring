#include "CisternMonitoring_HallSensorLib.h"
#include <RunningMedian.h>

//Constructor
HallSensor::HallSensor(unsigned int TRIG_PIN, unsigned int ECHO_PIN, unsigned int SENSORRANGE, unsigned int NUMOFMEASUREMENTS):
MedianFilter(3)
{
    
       _TRIG_PIN  = TRIG_PIN;
       _ECHO_PIN = ECHO_PIN;
       _SENSORRANGE = SENSORRANGE;
       _NUMOFMEASUREMENTS = NUMOFMEASUREMENTS;   
       
}
int HallSensor::getDistance(){
  for (int i=1; i <= _NUMOFMEASUREMENTS; i++){
    //measure Distance with Hall Sensor impulse
  digitalWrite(_TRIG_PIN, LOW);
  delayMicroseconds(15);// delayMicroseconds(2);
  digitalWrite(_TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(_TRIG_PIN, LOW);

   long duration = pulseIn(_ECHO_PIN, HIGH);
  int distance = duration*0.034/2;
  MedianFilter.add(distance);
  Serial.print("Measured Distance(raw): ");
  Serial.println(distance);
  }
  int distance_medianfiltered = (int)MedianFilter.getMedian();
  Serial.print("MeasuredDistance (median filtered): ");
  Serial.println(distance_medianfiltered);
  MedianFilter.clear();
  return distance_medianfiltered;
 
}
