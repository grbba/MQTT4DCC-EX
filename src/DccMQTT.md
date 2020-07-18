# DccMQTT Documentation {#mainpage}

Documentation for the DccMQTT support for the DCC-EX CommandStation

## Introduction

All of the following is based on the DCC code from Asbelos which you can find in his [GitHub repository](https://github.com/Asbelos/CVReader "CVeader by Asbelos")
Currently the DCC-EX command station communicates mainly on a point to point basis either through a serial line or WiFi ( based on the ESP8266 ). DCC-EX allows for receiving JRMI formatted command strings such ass \<R 1 1 1\> which reads the value of the configuration variable #1 from the decoder over Serial and WiFi (Standard Ethernet is not supported yet for receiving JRMI commands but should be easy to add).
WiThrottle/EngineDriver support has been added and can be used over WiFi but again is limited to the local network either directly or through the JRMI WiThrottle server if JRMI is installed on a computer on the local network with the CommandStation connected through a Serial line or over WiFi.

## Rationale

Why MQTT as a protocol for communicating with the DCC-EX base station? 

The vision for DCC-EX being to render DCC accessible to everyone the most simple and easy way. Being based on cheap Arduino hardware brought up the idea to consider the DCC-EX command station as an IoT device with as easy setup as e.g. for home automation appliances. Some of the usability criterias are

- Minimal hardware configuration ( but that is part of the DCC-EX CommandStation work to remove e.g. the need for jumpers ) 
- Mostly automatic software configuration
- Minimal to no network setup. No mDNS / bonjour services specificities router configurations etc...

Other criteria / ideas

- Get more devs doing frontend or any other work on top of it the most easy & simple way thus enlarging the the installed base of DCC++EX
- Enable the use of all UI Tooling available based on MQTT opening up more
- Be able to make a more modern version of JRMI (potentially without any installation as a web service)

Basically propose the same plug & play user experience you get for most digitally enabled equipment you can buy today being software only or having a hardware piece of equipment.


## Use case scenarios

For Individuals

maybe less of interest an only if they would be ready to pay a little amount / month for the RaaS ( rail road as a service ) product as everything to manage the layout would be on the cloud except the commandstation and depending on the commandstation with ESP wifi support (Where we have to figure out hwo to do the WiFI setup without recompilation ...)


For Clubs
managing multiple command stations with one inface and transparent switiching between all off them based on used identifable ressources. Today a loco is identified by its CV and you can not have more than one with thesame CV on the same layout. Taking one loco from one layout to another will yield possibly unwnated results. 

When taking a unkown loco to a new layout automatic reassignment of adresses can take place e.g. lookng up the assigned identities to that layout with their adresses find a free one a assign it to the new loco if there is a conflict.

So to start of the loco would be placed on the programming track and on the user expirience side only some sor of register loco to layout would have to be done everything else is transparent no cv to be used anymore and i don't have to know anything about the adresses of the other locos. 

Actually you could assign the loco to multiple layouts and just by putting the loco on the programming track and activate it may be sufficient ( we can even think about a fingerprinting system uniquley identifying the loco ? )

Manage multiple users on the same layout e.g. you want that a given user has only cpacity to control his/her own loco or a subset of locos/turnouts etc.. and the admins want minimal setup configuration to be done on this. In such a case we can imagine a topic per loco identity on the MQTT broker which then controls the acces rights based on the connected user. grant acces to a commandstation based on its uniqueID and on finer granularity grant acces to a loco assigned to the layout based on on it's identiy ( not the adress as they are transparent )

Going from club to club or better saying from layout to layout using the same setup gets transparent when used in the cloud where each user has his/her loco identities available. The admin of the layout then just has to assign the grants to a user if he isn't known yet

WiFI setup with smartphone
ESP8266 slmart config
