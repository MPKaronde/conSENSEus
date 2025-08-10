/*
To be implemented by all distance sensors used for communication with conSENSEus
*/

#ifndef DISTANCESENSORBASE_H
#define DISTANCESENSORBASE_H

class DistanceSensorBase {
    public:
        
        /*
        set enablePin to correct pin or leave as -1 if not used
        set minAccurateValue to whatever is determined in testing to prevent setting sensorWorking false on accident
        NOTE: will likely need an object of your particular sensor. If so, initialize it here.
        */
        virtual void begin() = 0;

        /* 
        turn on sensor for reading & set sensorEnabled to true
        return false if sensor already enabled or couldnt be enabled
        */
        bool enable()
        {
            if(sensorEnabled)
            {
                return false;
            }
            sensorEnabled = true;
            return enable_Sensor();
        }  

        /*
        turn off sensor for reading and set sensorEnabled to false
        return false if sensor already disabled or couldnt be disabled
        */
        bool disable()
        {
            if(!sensorEnabled)
            {
                return false;
            }
            sensorEnabled = false;
            return disable_Sensor();
        } 

        // check if sensor is enabled
        bool is_enabled()
        {
            return sensorEnabled;
        }

        // for processing to determine if sensor is working or not
        void set_sensor_not_working()
        {
            sensorWorking = false;
        }

        // return if the sensor is working
        bool is_sensor_working()
        {
            return sensorWorking;
        }

        /* 
        Take a reading, return to the nearest mm. 
        return -1 if distance out of range.
        return -2 if some other detectable issue within sensor
        return -3 if sensor not enabled or sensor not working
        must call enable prior to calling this or errors may occur
        */
        virtual int take_reading()
        {
            if(!sensorEnabled)
            {
                return -3;
            }

            int ret = sensor_Reading();

            // sensor returned error reading
            if(ret == -2 || !sensorWorking)
            {
                sensorWorking = false;
                return -2;
            }

            return ret;
        }

        virtual ~DistanceSensorBase(){}

    protected:
        /*
        True if sensor enabled, false otherwise.
        NOTE: even if sensor has no ability to disable, switching this bool is an expected behavior for the class
        */
        bool sensorEnabled = false;

        // Set to false if sensor is known to be taking faulty readings
        bool sensorWorking = true;

        // Lowest distance that sensor can accurately measure in testing, set in begin
        int minAccurateValue = 0;

        // The pin to use to enable / disable this sensor. Ideally should be set in the begin method
        int enablePin = -1;

        /*
        implement these w/ logic to actually enable / disable the sensors as needed
        pass if sensor cannot be enabled / disabled
        mostly intended for i2c devices & devices w/ low power mode
        return true if sensor successfuly enabled / disabled, false otherwise
        */
        virtual bool enable_Sensor() = 0;
        virtual bool disable_Sensor() = 0;

        /*
        implement to actually take a sensor reading
        return -1 if data out of range, -2 if some other detectable issue
        */
        virtual int sensor_Reading() = 0;
};

#endif