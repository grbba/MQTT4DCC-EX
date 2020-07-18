#ifndef _DccMQTTCommandMsg_h_
#define _DccMQTTCommandMsg_h_

// for every MQ message recieved one pool item is used and the reference of the item pushed into the inComing queue;
// once the command has been processed the same pool item is pushed in to the out going queue and its message id changed
// once the response has been send the pool item is returned to the pool

#define MAXNOCMDS 6
/**
 * @brief Defines all the commands which can be send via MQTT messages
 * @see Protocol
 */
enum Commands
{
  READ,     //!< "read"
  WRITE,    //!< "write"
  POWER,    //!< power on/off
  THROTTLE, //!< "throttle"operate
  FUNCTION, //!< "function" manage functions on enabled equipment
  INVALID_C //!< everything else is invalid
};

#define MAXNOPARAMS 9
enum Parameters
{
  CV,
  VALUE,
  LCOCMOTIVE,
  SPEED,
  DIRECTION,
  FN,
  STATE,
  TRACK,
  BIT,
  INVALID_P
};

enum Tracks
{
  ALL = 'A',
  MAIN = 'M',
  PROG = 'P'
};

#define MAXCID 24 // maximum length of a correlation id

// max length of the message pool; balance to be found in order to avoid too many cycles where the pool is full
// and no new messgaes can be recieved. We have to check before reception if the pool is full if full request
// the broker to resend ?

#define MAXMSG 10

struct DccMQCmdMsg
{
  Commands cmd;                       // Command that has been send via the MQ message
  uint16_t params[MAXNOPARAMS] = {0}; // Prameters of the command; Commands allowed are listed in <Parameters> above
  uint8_t nParams = 0;                // number of parameters passed in order to distguish optional parameters for a given command
  char _cid[MAXCID];                  // correlation id of max length MAXCID
  /**
   * @todo if the result is -1 the we shold fetch the last error and send it also 
   */
  int result = 0;                     // result from a read operation; used for creating the results message
  bool free = true;                   // is the pool entry in use or not
  uint16_t msgId;                     // internal identifier for knowing in which queue the pool item currently is and sequenec number (maybe removed to save space)
  uint8_t mIdx;                       // index of the item in the pool
};

class DccMQTTCommandMsg
{
private:
  /* data */
public:
  static uint8_t lastPopedIn;  // last pool item poped from the incomming queue
  static uint8_t lastPopedOut; // last pool item poped from the outgoing queue

  static DccMQCmdMsg msg[MAXMSG];               // pool command messages; can hold MAXMSG items
  static uint8_t setMsg(Commands c, char *cid); // Sets inital values upon reception of an MQ message and reserves the pool item
  static void printCmdMsg(DccMQCmdMsg *cm);     // Debug only
  static void printCmdMsgPool();                // Debug only

  DccMQTTCommandMsg();
  ~DccMQTTCommandMsg();
};

#endif
