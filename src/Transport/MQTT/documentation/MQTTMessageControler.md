
# MQTT Message Controler

## Protocol
The following describes DCC MQTT structure. The first paragraph contains the topic structure and the second the message format accepted by the DccMQTT controller.

At this point in time the message structure is based on the JRMI command structure and the DCC API for making the mapping straight forward. More high level commands can be integrated as well as if needed a very small message footprint is possible as well at the cost of limited readability and more processing needed on the client side issuing the commands.

The protocol has two components to it: Topics and a specific syntax/semantic for messages send by the MQTT broker depending on the topic used. The format for messages send to any topic or published into any topic is JSON.

### Topic structure

All topics start with a term identifying the intent. _/command_ e.g. its intent is to send a command to the command station expecting or not a reply.
Existing intents are:

- /command : commands for operations / programming etc.
- /result : results coming back from previously issued commands
- /admin : for administrative purposes 
- /telemetry : used for sending heartbeat data; memory consumption; ack currents etc ... 
- /diag : used for pushing diagnostic, debug or error messages

#### /command & /result

The /command and /result intent topics are constructed the following way:

	command/<CSID>/[L|T|S|A]; will only be used for recieving MQTT messages representing a command.
	response/<CSID>; will only be used for publishing results either with the nuleric result when available or succes/fail

where `CSID` is the Unique ID obtained from the processor. The Unique ID is obtained during the setup routines and published to the /admin channel.
The next level in the topic structure represents the ressources we look at:


**L = Locomotive or Layout** everything which can be done with a loco or powering your layout. Anything abstracting away the address shall be done on the API server who connects to the MQTT broker for the distribution of commands / reception of values from CV reads. This is done to avoid storage/processing overhead to map names to loco addresses. The layout configuration and locos present on the layout are assumed to have been loaded as soon as the command station comes online through the API through some management topics yet to be defined. 
The commands or payload to be send to this topic are of the following structure

	{c:<command>, cid:<#>, p:<parameters>} with 
	
	<command> :: read|write|throttle|function|power
	
	the parameters for the different commands are:
	
	read <parameters> :: {cv:<#> [bit:<#>]}
	
	write <parameters> :: {track:[M|P], cv:<#>,[locomotive:<#>], [bit:<#>], value:<#>}
		if the bit number is provided value must be 0 or 1
		if the track is M locomotive has to be provided
		if the track is P the locomotive will be ignored if set
	
	throttle <parameters> :: {locomotive:<#>, speed:<#>, direction:[F|R]}
	
	function <parameters> :: {locomotive:<#>, fn:<#>, value:[1|0]}; 
		fn number allowed is between 0 and 28;  1 = ON; 0 = OFF
	
	power <parameters> :: {track:[M|P|A], state:[1|0]};  
		M = main track, P = Programming track, A = both; 1 = ON; 0 = OFF
	
	Parameters can be provided in any order.

	Examples:
	
	Reading CV1
	
	{"c":"read", "cid":"12345", "p":{"cv":"1"}} will read the value from CV1

	on the result topic the answer would be 

	{"result":3, "cid":"12345"};

	Power on all tracks:

	{"c":"power", "cid":"12345", "p":{"track":"A", "state":"ON"}} will power up both tracks

	on the result topic the answer would be 

	{"result":"succes", "cid":"12345"};

	<#> denotes an integer value
	
	Note(1):cid stands for correlation id. This is a unique number to be set by the sender allowing him to 
	link a message send on /command topic to the result recieved in the /result topic.
	
	Note(2): to self: switch track state or provide the track in every call who can handle both ? 


**T = Turnout** - operations for switch turnouts - TBD

**S = Sensor** - setting getting sensor values - TBD

**A = Accessory** - idem for Accesroes - TBD


#### /admin
To be done; All admin related stuff

#### /telemetry
To be done; collect operational metrics from the command station

#### /diag
To be done; publish diagnostic / debugging messages

#### /jmri
Accepts std JMRI commands in the  < > format. They will be directly passed on and processed by the DCC-EX commandstation; no error checking/reporting, diagnostics etc will be send to the relevant MQ topics.

## Design considerations
Message pooling and double queues one for incoming and one for outgoing messages as responses to commands. Both are handled asynchronously


