#ifndef _ETHNetwork_h_
#define _ETHNetwork_h_

#include <Ethernet.h>

/**
 * @brief Network Configuration
 * 
 */
#define MAC_ADDRESS { 0xA8, 0x61, 0x0A, 0xAE, 0x6A, 0x78 }     // MAC address of your networking card
#define IP_ADDRESS 10, 0, 0, 101                                // Just in case we don't get an adress from DHCP try a static one; make sure this one is not used elsewhere and corresponds to your network layout
#define LISTEN_PORT 80                                          // default listen port for the server 


class ETHNetwork
{
private:
    // Network Connection
    static EthernetServer server;
    static IPAddress ip;
    static byte mac[6];

    static void networkSetup();                         // get the Network up

public:
    static void setup();                                // main entry to get things going
    static void loop();

    static EthernetServer getServer() {
        return server;
    } 

    ETHNetwork();
    ~ETHNetwork();
};

#endif