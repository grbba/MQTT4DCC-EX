
/**
 * @file DccTelemetry.cpp
 * @brief  Telemetry class allowing to report metrics to a specific MQTT topic for monitoring/debugging etc. Either, by adding a metric, this class when it's loop is 
 * getting called reoprts all the available metric or it can be called individually. Two specific measurements are provided concerning memory consumption and time measurements.
 *
 * @version 0.1
 * @date 2020-07-08
 * @author Gregor Baues
 * @copyright Copyright (c) 2020
 *
 *  This is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  It is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details <https://www.gnu.org/licenses/>.
 */

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Transport/MQTT/DccMQTT.h>
#include <Diag/DIAG.h>
#include <Diag/Telemetry/DccTelemetry.h>
#include <avr/pgmspace.h>

bool DccTelemetry::active = false;
int DccTelemetry::time[TIMESERIES] = {0};
char DccTelemetry::telemetryTopic[] = {'\0'};
Measure DccTelemetry::measurements[MAX_NB_MEASURES] = {0};
byte DccTelemetry::idx = -1; // number of registered measurements; -1 there is none

#define PUB_FMT "{\"%s\":\"%d\"}"
PROGMEM const char f[] = {PUB_FMT};
PROGMEM const char dTfmt[] = {"{\"deltaT\":\"%d\"}"};
PROGMEM const char memFmt[] = {"{\"memory\":\"%d\"}"};

/**
 * @brief When called will calculate the difference of time passed for a given time series
 * 
 * @param t     : timeseries
 * @return int  : Returns the time interval between two calls to deltaT with the same time series t
 */
int DccTelemetry::deltaT(uint8_t t)
{
  int deltaT = millis() - DccTelemetry::time[t];
  DccTelemetry::time[t] = millis();
  if (DccTelemetry::active && deltaT > 20) // report anything above threshold of 20ms
  {
    char _memMsg[20];
    INFO(F("tDelta > 20ms: %d"), deltaT);
    sprintf_P(_memMsg, dTfmt, deltaT);
    DccMQTT::mqClient->publish(DccMQTT::topics[TELEMETRY], _memMsg);
  }
  return deltaT;
}

// publish free memory as reported in fm - to be replaced by telemetry
/**
 * @brief Publish free memory as reported in fm; Free memory is calculated by freeMemory()
 * 
 * @param fm 
 */
void DccTelemetry::free_memory(int fm)
{
  char _memMsg[20]; //!< string buffer for the serialized message to return

  /**
   * @todo check for buffer overflow !
   */

  if (DccMQTT::mqClient->connected())
  {
    sprintf_P(_memMsg, memFmt, fm);
    DccMQTT::mqClient->publish(DccMQTT::topics[TELEMETRY], _memMsg);
  }

  return;
}

/**
 * @brief Published directly a integer metric for cases where a periodic report is not appropriate
 * 
 * @param metric Name of the metric to report
 * @param v value to report
 */

void DccTelemetry::measure(char *metric, int v)
{
  char _memMsg[20]; //!< string buffer for the serialized message to return
  /**
   * @todo check for buffer overflow !
   * @todo add the signature where the metric is not the value but the function in order to obtain the metric
   */
  if (DccMQTT::mqClient->connected())
  {
    sprintf_P(_memMsg, f, metric, v);
    DccMQTT::mqClient->publish(DccMQTT::topics[TELEMETRY], _memMsg);
  }
  return;
}

/**
 * @brief adds a measurement to the measurements array
 * 
 * @param m The function which to call to obtain the value to report
 * @param name Name of the Metric; Shall not be longer than MAX_M_NAME
 * @return true successfully added
 * @return false Metric hasn't been added becasue the name was too long or the max number of metrics has been exceeded
 */
bool DccTelemetry::addMeasure(Metric m, char *name)
{
  if (strlen(name) > MAX_M_NAME)
    return false; // name too long
  /**
     * @todo add error message and check for max no of metrics
     * 
     */
  idx++; // new metric to be added;
  strcpy(*DccTelemetry::measurements[idx].name, name);
  DccTelemetry::measurements[idx].metric = m;
  DccTelemetry::measurements[idx].var = NULL;
  return true;
}

/**
 * @brief Publish a measure from the measurements array
 * with some more text
 * @param i : <b>Index</b> of the measure to be send
 */

void publishMeasure(byte i)
{
  char _memMsg[20];         // string buffer for the serialized message to return

  if (DccMQTT::mqClient->connected())
  {
    Measure *m = DccTelemetry::getMeasure(i);
    sprintf_P(_memMsg, f, *m->name, *m->var);
    DccMQTT::mqClient->publish(DccMQTT::topics[TELEMETRY], _memMsg);
  }
}

/**
 * @brief In the loop for the telemetry all measures registered by addMeasure are send out
 * 
 */
void DccTelemetry::sendMeasure()
{
  if (idx == -1)
    return; // no metrics have been registered;
  if (DccTelemetry::isActive())
  {
    for (int i = 0; i <= idx; i++)
    {
      publishMeasure(i);
    }
  }
}

void DccTelemetry::setup()
{
  DccTelemetry::state(true); // tells the Telemetry object to send out metrics
}

bool DccTelemetry::state(bool s)
{
  DccTelemetry::active = s;
  return s;
}

void DccTelemetry::loop()
{
  DccTelemetry::sendMeasure(); // publishes current state of all previously registered metrics
}

DccTelemetry::DccTelemetry()
{
}

DccTelemetry::~DccTelemetry()
{
}