/**
 * @file DccMQTTProc.cpp
 * @author Gregor Baues (gr2bba@gmail.com)
 * @brief Eexecuting DCC commands recieved through via MQTT. This class relies on the DCC API from the CVReader project by Asbelos
 * @version 0.1
 * @date 08-07-2020
 * 
 * @copyright Copyright (c) 2020
 * 
 * Licenced under GPLv3
 */

#include <Arduino.h>
#include <Diag/DIAG.h>

#include <Transport/MQTT/DccMQTTCommandMsg.h>
#include <Transport/MQTT/DccMQTT.h>
#include <Transport/MQTT/DccMQTTProc.h>

#define CVREADER 1        // Chris CVeader
#define DCC_PP_EX 2       // current DCC++ EX release
#define COMMAND_STATION 3 // Davids CommandStation implementation

#define DCC_ENGINE CVREADER // The MQTT message processor only supports CVREADER for now

/**
 * @todo add support for DCC++ EX and CommandStation
 */

#if DCC_ENGINE == CVREADER
#include <DCC.h>
#include <DCCWaveform.h>
#endif

//-----------
// Defines
//-----------

#define MAXRETRIES 4 // number of retroies allowed in case we get a bad ACK for DCCAPI functions relying on this

//-----------
// Variables
//-----------

/**
 * @brief Contains the pool array index of the last message poped from the incomming queue
 */
uint8_t DccMQTTCommandMsg::lastPopedIn;
bool inCallback = false;

//-----------
// Functions
//-----------

/**
 * @brief Function called by all DCC API calls which require an ACK
 * 
 * @param result The result value from the executed DCC Api call. Will be -1 in case of failure
 */
void dccCallback(int result)
{
    
    DBG(F("dccCallback: lastpopedIn: %d current result: %d"), DccMQTTCommandMsg::lastPopedIn, result);
    DccMQTTCommandMsg::msg[DccMQTTCommandMsg::lastPopedIn].result = result;
    DccMQTT::pushOut(DccMQTTCommandMsg::lastPopedIn); // push the msg back into the outgoing queue
    inCallback = false;

}

void DccMQTTProc::loop()
{
    if (DccMQTT::inIsEmpty())
    {
        return;
    }
    if (inCallback)
    {
        return;
    }

    DccMQTTCommandMsg::lastPopedIn = DccMQTT::popIn();
    int midx = DccMQTTCommandMsg::lastPopedIn;
    byte result = 1;

    // process the message
    switch (DccMQTTCommandMsg::msg[midx].cmd)
    {
        case READ:
        {
            inCallback = true;
            switch (DccMQTTCommandMsg::msg[midx].nParams)
            {
                case 1:
                { // read Byte
                    DCC::readCV(
                        DccMQTTCommandMsg::msg[midx].params[CV],
                        dccCallback
                    );
                    break;
                }
                case 2:
                { // read Bit value from CV
                    DCC::readCVBit(
                        DccMQTTCommandMsg::msg[midx].params[CV],
                        DccMQTTCommandMsg::msg[midx].params[BIT],
                        dccCallback
                    );
                    break;
                }
                default: {
                    ERR(F("Read: Wrong number or parameters provided"));
                }
            }
            return; // the results are published in the dccCallback!
        }
        case WRITE:
        {
            
            switch(DccMQTTCommandMsg::msg[midx].nParams) {
                case 2:{  // only CV and value so we assume progtrack with one loco and ACK
                    inCallback = true;
                    DCC::writeCVByte(
                        DccMQTTCommandMsg::msg[midx].params[CV],     
                        DccMQTTCommandMsg::msg[midx].params[VALUE],
                        dccCallback               
                    );
                    return; // the results are published in the dccCallback!
                }
                case 3: {  // as 2 but with bit write set
                    inCallback = true;
                    DCC::writeCVBit(
                        DccMQTTCommandMsg::msg[midx].params[CV],  
                        DccMQTTCommandMsg::msg[midx].params[BIT],   
                        DccMQTTCommandMsg::msg[midx].params[VALUE],
                        dccCallback               
                    );
                    return; // the results are published in the dccCallback!
                }
                case 4: {   // loco and track provided should be main at this point no bit write
                    DCC::writeCVByteMain(
                        DccMQTTCommandMsg::msg[midx].params[LCOCMOTIVE],
                        DccMQTTCommandMsg::msg[midx].params[CV],
                        DccMQTTCommandMsg::msg[midx].params[VALUE]
                    );
                    break;
                }
                case 5: {   // as 4 but with writing value 0 or 1 at bit number provided
                    DCC::writeCVBitMain(
                        DccMQTTCommandMsg::msg[midx].params[LCOCMOTIVE],
                        DccMQTTCommandMsg::msg[midx].params[CV],
                        DccMQTTCommandMsg::msg[midx].params[BIT],
                        DccMQTTCommandMsg::msg[midx].params[VALUE]
                    );
                    break;
                }
                default: {
                    ERR(F("Read: Wrong number or parameters provided"));
                }
            }
            result = 1;
            break;
        }
        case POWER:
        {
            switch (DccMQTTCommandMsg::msg[midx].params[TRACK])
            {
            case 'A':
                DBG(F("Power ALL : %d"), DccMQTTCommandMsg::msg[midx].params[STATE]);
                DCCWaveform::mainTrack.setPowerMode(
                    (POWERMODE)DccMQTTCommandMsg::msg[midx].params[STATE]
                );
                DCCWaveform::progTrack.setPowerMode(
                    (POWERMODE)DccMQTTCommandMsg::msg[midx].params[STATE]
                );
                break;
            case 'M':
                DBG(F("Power MAIN : %d"), DccMQTTCommandMsg::msg[midx].params[STATE]);
                DCCWaveform::mainTrack.setPowerMode(
                    (POWERMODE)DccMQTTCommandMsg::msg[midx].params[STATE]
                );
                break;
            case 'P':
                DBG(F("Power PROG : %d"), DccMQTTCommandMsg::msg[midx].params[STATE]);
                DCCWaveform::progTrack.setPowerMode(
                    (POWERMODE)DccMQTTCommandMsg::msg[midx].params[STATE]
                );
                break;
            }
            break;
            result = 1;
        }
        case THROTTLE:
        {
            DBG(F("Throttle: %d %d %d "), DccMQTTCommandMsg::msg[midx].params[LCOCMOTIVE],
                DccMQTTCommandMsg::msg[midx].params[SPEED],
                (bool)DccMQTTCommandMsg::msg[midx].params[DIRECTION]);

            DCC::setThrottle(
                DccMQTTCommandMsg::msg[midx].params[LCOCMOTIVE],
                DccMQTTCommandMsg::msg[midx].params[SPEED],
                (bool)DccMQTTCommandMsg::msg[midx].params[DIRECTION]
            );
            result = 1;
            break;
        }
        case FUNCTION:
        {
            DBG(F("Function: %d %d %d "),DccMQTTCommandMsg::msg[midx].params[LCOCMOTIVE],
                DccMQTTCommandMsg::msg[midx].params[FN],
                DccMQTTCommandMsg::msg[midx].params[STATE]);
            DCC::setFn(
                DccMQTTCommandMsg::msg[midx].params[LCOCMOTIVE],
                DccMQTTCommandMsg::msg[midx].params[FN],
                DccMQTTCommandMsg::msg[midx].params[STATE]
            );
            result = 1;
            break;
        }
        case INVALID_C:
        {
        ERR(F("Invalid command recieved"));
        return;
    }
    }

    DccMQTTCommandMsg::msg[DccMQTTCommandMsg::lastPopedIn].result = result;
    DccMQTT::pushOut(DccMQTTCommandMsg::lastPopedIn); // push the msg back into the outgoing queue    
}

DccMQTTProc::DccMQTTProc()
{
}

DccMQTTProc::~DccMQTTProc()
{
}