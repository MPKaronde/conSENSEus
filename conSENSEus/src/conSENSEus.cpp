#include "conSENSEus.h"

// constructor behavior no-op
conSENSEus::conSENSEus(){}

// initial setup stuff
void conSENSEus::begin(int sensorCount, int sensorTolerance, int readingTolerance)
{
    // set numSensors
    this->numSensors = sensorCount;

    // set readingTolerance
    this->readingTolerance = readingTolerance;

    /*
    set sensor tolerance
    if value invalid, set to sensorCount - 1
    */
    if(sensorTolerance > sensorCount - 1 || sensorTolerance < 0){sensorTolerance = sensorCount - 1;}
    this->sensorNumTolerance = sensorTolerance;

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

/*
Takes readings from all sensors, returns the decided reading
Return -1 if out of range of all sensors
If critical mass of sensor not working, return -2
*/
int conSENSEus::take_reading()
{
    int readings[this->numSensors];
    int brokenSensors = 0;  // num sensors that recorded errors
    int outOfRanges = 0;    // num sensors that recorded distance out of range
    int validReadings = 0;  // num sensors that recorded something other than error or out of range
    float validAverage = 0; // running count / average of all valid readings
    for(int i = 0; i < this->numSensors; i++)
    {
        DistanceSensorBase* sensor = this->sensors[i];
        sensor->enable();
        readings[i] = sensor->take_reading();
        sensor->disable();

        if(readings[i] == -1){outOfRanges++;}           // reading out of range
        if(readings[i] == -2){brokenSensors++;}         // sensor is broken
        if(readings[i] >= 0) {    // reading is valid
            validReadings++;
            validAverage += (float)readings[i];
        }
    }

    // too many broken sensors, cant report data
    if(brokenSensors >= this->sensorNumTolerance){return -2;}

    // all sensors report out of range
    if(outOfRanges = this->numSensors - brokenSensors) {return -1;}

    // calculate valid readings if possible
    if(validReadings > 0)
    {
        validAverage = validAverage / (float)validReadings;
    }

    // look for outliers to boot from average and recalculate
    int upperLimit = (int)validAverage + this->readingTolerance;
    int lowerLimit = (int)validAverage - this->readingTolerance;
    float newAverage = 0;
    int newNumValid = 0;
    for(int i = 0; i < this->numSensors; i++)
    {
        if(readings[i] < 0){continue;}   // invalid to our purposes here

        if(readings[i] <= upperLimit && readings[i] >= lowerLimit)
        {
            newAverage += readings[i];
            newNumValid++;
        }
        /*
        NOTE: Uncomment the following if you're sensors are generally quite accurate. it can catch smaller mistakes
            If they are not however (ie: usual hobby sensors), uncommenting this will just lead to annoying errors
        else{this->sensors[i]->set_sensor_not_working();}
        */
    }

    newAverage = newAverage / (float)newNumValid;
    return (int)newAverage;    
}


