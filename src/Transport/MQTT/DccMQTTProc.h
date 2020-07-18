#ifndef _DccMQTTProc_h_
#define _DccMQTTProc_h_

#include <DCC.h>

// Interface class between the MQTT transport and the DCC API
// All incomming commands / results will be managed here

  /* Public DCC API functions
  static void setThrottle( uint16_t cab, uint8_t tSpeed, bool tDirection);
  static uint8_t getThrottleSpeed(int cab);
  static bool getThrottleDirection(int cab);
  static void writeCVByteMain(int cab, int cv, byte bValue);
  static void writeCVBitMain(int cab, int cv, byte bNum, bool bValue);
  static void setFunction( int cab, byte fByte, byte eByte);
  static void setFn( int cab, byte functionNumber, bool on);
  static void setAccessory(int aAdd, byte aNum, bool activate) ;
  static bool writeTextPacket( byte *b, int nBytes);
  static void setDebug(bool on);
  
  // ACKable progtrack calls  bitresults callback 0,0 or -1, cv returns value or -1 
  static void  readCV(int cv, ACK_CALLBACK callback);
  static void  readCVBit(int cv, byte bitNum, ACK_CALLBACK callback);  // -1 for error
  static void writeCVByte(int cv, byte byteValue, ACK_CALLBACK callback) ;
  static void writeCVBit(int cv, byte bitNum, bool bitValue, ACK_CALLBACK callback);
*/

class DccMQTTProc
{
private:
    
public:
    static void loop();

    DccMQTTProc();
    ~DccMQTTProc();
};


#endif