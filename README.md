# MQTT4DCC-EX

An MQTT interface for [DCC++EX](https://dcc-ex.github.io/)  

** WARNING ** This is alpha stage code and not for any other use than testing MQTT connections to a DCC command station either from the internet or a local MQTT broker.

## What you need

- An Arduiono Mega ( preffered not yet tested on anything else )
- A compatible MotorShield 
- An EthernetShield
- A MQTT Client App

The first two components make up your command station and should be put together following the guidelines here : [DCC++EX](https://dcc-ex.github.io/)
You will not need any software from the DCC++EX repo or download from the website. The DCC part is embedded in the MQTT poc here and based on the work from [Asbelos](https://github.com/Asbelos/CVReader)

*Note*: Currently this only works on an Arduino with EthernetShield no WiFi support yet out of the box. 

## Parameter settings:

Once having that sorted out you need to make some config modifications: 

Fist edit DccMQTT.h and change according to your broker

- #define MQTT_BROKER_PORT 1883 
- #define MQTT_BROKER_DOMAIN "your mq tt broker on the web.xxx"  
- #define MQTT_BROKER_ADDRESS "IP address of the  broker" 
- #define MQTT_BROKER_USER "username"
- #define MQTT_BROKER_PASSWD "password"

At this point in time only the MQTT_BROKER_ADDRESS, PORT, USER and PASSWD is used. I assume that your borker requires username and password. If that is not the case have to change the line for the connection to the broker to the same as for a local install of MQTT without any user/passwd & acl setup.

Any broker will do as long as it supports MQTT version 3 which most even public ones should support.
Or depending if you defined CLOUDBROKER to 0 the second set with you local installation of e.g. mosquitto as broker

**Note**: The Arduino unique Id is used for building the topics to subscribe to. On any public broker without any user authentication this **ID would be visible**. User Authentication shall be added soon !

Second edit Transport/Ethernetwork.h and change according to your network setup

- #define MAC_ADDRESS {  0xAA, 0x11, 0x22, 0xEE, 0xFF, 0x88 }     // MAC address of your networking card
- #define IP_ADDRESS 10, 0, 0, 101                                // Just in case we don't get an adress from DHCP try a static one; make sure this one is not used elsewhere and corresponds to your network layout
- #define LISTEN_PORT 80                                          // default listen port for the server

Normally only the MAC address should be set as DHCP from your Router/Modem or ISP provided equipment will provide the proper address. The IP_ADDRESS here is just a fallback in case the DHCP doesn't respond. In this case you will have to find out the network configuration and make sure that the address provided is not used by any other equipment.

Compile in Visual Studio Code and Platform.io and upload. The system will start in a very verbose mode and connect to the Broker. In order to change this edit Diag/Diag.h and set LOGLEVEL accordingly.

Using an MQTT client such as MQTT Explorer or MQTTfx you should be able to send commands as described in the documentation [here](https://grbba.github.io/MQTT4DCC-EX/html/md_mqtt4dcc_09_09ex_src__transport__m_q_t_t_documentation__m_q_t_t_message_controler.html)


