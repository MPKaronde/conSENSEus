#include "conSENSEus.h"

// constructor behavior no-op
conSENSEus::conSENSEus(){}

// initial setup stuff
void conSENSEus::begin(int sensorCount)
{
    // set numSensors
    this->numSensors = sensorCount;

    // initialize sensors to correct length and as nullptrs for safety
    this->sensors = new DistanceSensorBase*[sensorCount];
    for(int i = 0; i < sensorCount; i++)
    {
        sensors[i] = nullptr;
    }
}

/*
Takes in pointer to given sensor and the index to store it
Return true if sensor successfuly stored
return false if index invalid or some other error occured
*/
bool conSENSEus::set_sensor(int index, DistanceSensorBase* base)
{
    if(index < 0 || index >= this->numSensors)
    {
        return false;
    }
    this->sensors[index] = base;
    return true;
}


