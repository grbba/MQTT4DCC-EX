/**
 * @file DCCppEX_MQTT.cpp
 * @author Gregor Baues
 * @author Chris Harlow
 * @brief  Main entry point for the DCC++EX CommandStation MQTT interface. Derived from CVReader.ino from Chris Harlow. 
 * By removing the #define DCCMQTT you will get the CVReader only running. The DCC++EX project is referenced below as well
 * @see <https://github.com/Asbelos/CVReader>
 * @see <https://github.com/DCC-EX>
 * @see 
 * @version 0.1
 * @date 2020-07-08
 * 
 * @copyright Copyright (c) 2020; All rights reserved 
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


#include <DCC.h>
#include <Diag/DIAG.h>
#include <DCCEXParser.h>
#include <WifiInterface.h>
#include <MotorDrivers.h>

#define DCCMQTT //<! comment this line in order to compile only CVReader without any MQTT support

#ifdef DCCMQTT
#include <ETHNetwork/ETHNetwork.h>
#include <Transport/MQTT/DccMQTT.h>
#include <Transport/MQTT/DccMQTTProc.h>
#endif

void myFilter(Print &stream, byte &opcode, byte &paramCount, int p[])
{
  switch (opcode)
  {
  case 'F': // Invent new command to call the new Loco Function API <F cab func 1|0>
    INFO(F("Setting loco %d F%d %S"), p[0], p[1], p[2] ? F("ON") : F("OFF"));
    DCC::setFn(p[0], p[1], p[2] == 1);
    opcode = 0; // tell parser to ignore this command
    break;
  case '#': // Diagnose parser <#....>
    INFO(F("# paramCount=%d"), paramCount);
    for (int i = 0; i < paramCount; i++)
      INFO(F("p[%d]=%d (0x%x)"), i, p[i], p[i]);
    opcode = 0; // Normal parser wont understand #,
    break;
  default: // drop through and parser will use the command unaltered.
    break;
  }
}

// Callback functions are necessary if you call any API that must wait for a response from the
// programming track. The API must return immediately otherwise other loop() functions would be blocked.
// Your callback function will be invoked when the data arrives from the prog track.
// See the DCC:getLocoId example in the setup function.

void myCallback(int result)
{
  INFO(F("Getting Loco Id callback result=%d"), result);
}

DCCEXParser serialParser;

void setup()
{
  INFO(F("Setup started ...")); // Connect to the Network
  Serial.begin(115200); // Responsibility 1: Start the usb connection for diagnostics and possible JMRI input
  INFO(F("Serial is up ..."));
  DCC::begin(STANDARD_MOTOR_SHIELD);         // Responsibility 2: Start the DCC engine.
  INFO(F("DCC engine running ..."));

#ifdef DCCMQTT
  ETHNetwork::setup();
  INFO(F("Network connected ...")); // Connect to the Network
  DccMQTT::setup(serialParser);
  INFO(F("MQTT broker connected ...")); // Connect to the MQTT broker
#endif

#ifdef WIFI
  WifiInterface::setup(Serial1, F("BTHub5-M6PT"), F("49de8d4862"), 3532); // (3532 is 0xDCC decimal... )
#endif

  DCCEXParser::setFilter(myFilter); // Optionally tell the command parser to use my example filter. This will intercept JMRI commands from both USB and Wifi
  INFO(F("Ready for JMRI commands"));
}

void loop()
{

  DCC::loop();               // Handle DCC background processes; loco reminders and power checks
  serialParser.loop(Serial); // Handle any incoming commands on USB connection

#ifdef DCCMQTT
  DccMQTT::loop(); // Handle any MQTT message
#endif

#ifdef WIFI
  WifiInterface::loop(Serial1); // Responsibility 3: Optionally handle any incoming WiFi traffic
#endif
}