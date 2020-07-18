# DCC++ EX MQTT principles

## Overview
The following schema describes the logical setup for using the DCC++EX MQTT connector:
<center>
![Base MQTT architecture](MQTTbase.png "DCC++ EX MQTT setup")

Base MQTT architecture
</center>

1. A client application connects to the MQTT broker and subscribes to one or more topics. Topics can be seen as mailboxes to which i can send and/or recieve a message.
2. The DCC++EX command station as well connects to the broker and subcribes to one or more topics.
3. The command station (together with the motor shield) is connected to your tracks


If the client application and the DCC++EX command station subscribe to the same topic they can exchange messages through this topic. The client application can issue coammands whcih will be recieved by the commandstation and get executed. The client will recive a message with the the result e.g. when reading an configuration variable from a decoder of a locomotive on the programmming track.

The toipc structure of the DCC++EX MQTT connectos is described here: 