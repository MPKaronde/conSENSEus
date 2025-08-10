/*
    Defines that conSENSEus class to determine the best measurement of multiple distance sensors
*/

#ifndef conSENSEus_H
#define conSENSEus_H

#include "DistanceSensorBase.h"
#include <Arduino.h>

class conSENSEus {
    public:
        void begin (int sensorCount){};                          // called on startup
        bool set_sensor(int index, DistanceSensorBase* sensor);  // attach each sensor, return false if index invalid
        int take_reading(){};       // take readings from all sensors. return decided range, -1 if out of range
        bool system_working(){};    // checks all the sensors, return false if any sensor is not working 
        bool sensorEnabled(int index){};    // return true if sensor at given index is enabled
        bool sensorWorking(int index){};    // return false if sensorWorking is false for given sensor   

    private:
        DistanceSensorBase** sensors;   // array of pointers to the sensors
        int numSensors;                 // num sensors being used      
};

#endif
