#ifndef _DccTelemetry_h_
#define _DccTelemetry_h_

#include <Arduino.h>

#define MAX_M_NAME          10     // max length for the name of a measurement
#define MAX_NB_MEASURES     20     // max number of different measurements i can send
#define TIMESERIES          10     // number of timing points 

typedef int (*Metric)();

enum Measurement 
{
    MEMORY,
    ACK_CURRENT,
    MQTT_MSG_TIMING,
    MQTT_CMD_PROC_TIMING
};

struct Measure {
    int *var;
    char *name[MAX_M_NAME];
    Metric metric;
};

class DccTelemetry
{
private:
    static bool active;                                     // by default telemetry is on
    static Measure measurements[MAX_NB_MEASURES];           // array of measurements pointers to ints ( the variables to be measured collected )
    static int time[TIMESERIES];                            // contains the the last measurement in millis for timestamp
    static char telemetryTopic[30];                         // Topic to publish telemetry data to

    static void sendMeasure();

public:
    
    static byte idx;

    static void setup();                             // sets up the topic  
    static bool state(bool s);                       // switches on/off the telemetry transmission returns current state 
    static void loop();                              // allow for collection/publish of measurements to the telemetry channel
    static bool addMeasure(int *var, char *name);    // register a varaible to be measured and its name. At each cycle the loop will fetch the value of var and push it to the topic  
    static bool addMeasure( Metric m, char *name);   // add a function which will be called to obtain the value to report
    static int  deltaT(uint8_t t);                   // reports back the delta t between the current call and the last call (value stored at index t in time )
    static void free_memory(int fm);                 // send the amount of memory left between stack and heap
    static void measure(char* metric, int v);        // metric contains the name used in the metric send; v is the value send for the metric


    static Measure *getMeasure(byte i) {
        return &measurements[i];
    }

    static bool isActive() {
        return active;
    }

    DccTelemetry();
    ~DccTelemetry();
};

#endif
