
# Deployment patterns

MQTT broker
- On premise: Install on your own machine PC/MAC/Linux e.g. mosquitto (free using opensource brokers)
- Cloud: three options
1. on your own server. much like the on premise installexcept maybe security as anyone can access at thispoint in time some config for obfuscation can be done ( e;g. change std ports / req logon etc ... ) needs definitley moer attention than the on premise one. (but € required)
2. Use readily availabe test brokers like HiveMQ or Mosquitto ( free but insecure and may apply throtteling )
3. Use devops environemnt like AWS or Azure which have MQTT instances available (but € are required)

OpenSource is free but that doesn't mean it cost you nothing :) 


Client App
    Frontend or server app
    anywhere PC, Tablet, Phone, server as long as the app supports MQTT v3 and up.
    as a starter you can use MQTT client apps such as MQTTfx or MQTT Explorer 

DCC++EX commandstation
    well thats close a layout so on premise only. No other option possible
    either connected over serial, Ethernet or WiFi to a computer and/or Network


## User Expirience:

Goal is to get as close as possible to ZERO configuration by the user. Some items will be unavoidable through the environemnt such as WiFi and MQ broker security related point (uid/pwd at minimum)

### Padawan:
1. Get arduino plus motorshield as labeled supported on DCC++EX website
2. assemble the whole thing following the instructions and hook it up to the layout and your PC/MAC/Linux box using a USB cable
    ? How do i know if it works? if i can figure it out what to do when it doesn't work
3. Download installer and run the installer (Will figure out conection details automatically)

    Done for the DCC-EX part

    Then user is on his own for using JRMI / or anything else refer to the website for those tools ( here the UX breaks imho and we get into the wizardy environment - The JRMI website is fu... intimidating - maybe we should add a download section on the website for that one)


even more Padawan:
    OR
    get preinstalled firebox

    // note to self:
    // preinstallation process requires to compile the code with the device ID in a #define so needs a script
    // upload sketch to obtain unique ID
    // compile and upload the command station together with the uniqueID -DUNIQUEID=<result obtained from before>

    hook it up to the layout and fire it up


### Security vs Device considerations
In the line up of supported command station configurations only the configurations using WiFi support security whilst in transit i.e. TLS/SSL. The Arduinos do not have the power for the required calculations in order to run the Security protocols. Research has shown various initatives to provide TLS/SSL exist(ed) but none readily accessible ONLY ESP based WifI connection is capable of doing so. 

TLS/SSL support
                 Serial Ethenet WiFi(ESP)      
Command station     n/a    -          x

Serial is not considered here as its point to point with little man in the middle attacks possible.

That means without any security degradation only the Wifi option would be available to fully secure the whole flow when a cloud based broker is used. (very) limited security can be provided by obfuscating the application protocol so that it gets expensive to figure out how it communicates ( but its open source so anyone who wants and spends the effort can look at the code and get it ) So thelimited part will come from some basic encyption to be done on the payloads send around and that would require the decryption on the other end which off the shelf client apps will not be able to do so we need an additional backend component to make that work .... ( chained revers proxy brokers/topics  with the decryption/encryption in the middle ... ?? )

It would be good practice to that also on premise as when TLS/SSL everything gets over thewire in clear text incl. your Wifi password when using the std WifIclient ... There is no reason not to put TLS in place as in the current setups all devices used for client apps and/or JRMI are TLS/SSL capable.

Operate a small broker for people to use freely to develop apps if it scales start asking for a small fee to pay the infra comming with it ..


