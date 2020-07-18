#include <Arduino.h>
#include <ETHNetwork/ETHNetwork.h>

#include <Diag/DIAG.h>


EthernetServer ETHNetwork::server = EthernetServer(LISTEN_PORT); // Ethernet Server listening on port LISTEN_PORT
IPAddress ETHNetwork::ip = IPAddress(IP_ADDRESS);                // IP address needed to get to the server
byte ETHNetwork::mac[6] = MAC_ADDRESS;

// Functions
void ETHNetwork::setup()
{
    ETHNetwork::networkSetup();  // get the Network up and running

}

// Get the Network up
void ETHNetwork::networkSetup()
{
    int error;

    // setup the networking stack
    error = Ethernet.begin(mac); // setup the Ethernet stack

    if (error == 0)
    { // DHCP didn't provide an address
        INFO(F("Initalizing with fixed ip adress ..."));
        Ethernet.begin(mac, ip);
    }

    delay(1000);            // grace period to get the Networking stack up
    server.begin();
    IPAddress ip = Ethernet.localIP();
    INFO(F("The local IP address is: %d.%d.%d.%d "), ip[0], ip[1], ip[2], ip[3]);
    ip = Ethernet.dnsServerIP();
    INFO(F("The DNS server IP address is: %d.%d.%d.%d "), ip[0], ip[1], ip[2], ip[3]);
}


void ETHNetwork::loop()
{
    // EthernetClient client = server.available(); // no need for now
}

// Constructor and Destructor
ETHNetwork::ETHNetwork()
{
}

ETHNetwork::~ETHNetwork()
{
}