# Scissors

*Scissors* is a simple string parsing library for Arduino.  Strings can be locally generated or recieved from *Streams*.

*Scissors* plays well with its sibling library [Glue - https://github.com/hex705/Glue](https://github.com/hex705/Glue).  Glue and Scissors are also available for Processing.

As of August 2012, Scissors is an alpha release.  It is tested and appears stable.  Please report bugs here:

[https://github.com/hex705/Scissors/issues](https://github.com/hex705/Scissors/issues "Issues")

Library [dev history](#history) can be found at bottom of this file.


Quick Start
---------------

1. Obtain archive (.zip) @  [https://github.com/hex705/Scissors](https://github.com/hex705/Scissors "Download")

2. Unzip and (if needed) rename folder Scissors

3. Copy folder (OSX) into:   ~/Documents/Arduino/libraries

4. Start cutting data streams with *Scissors*.

Downloaded folder contains library, and examples.


Usage
-----

Scissors expects structured messages with packages of the following form (spaces for clarity ONLY - do not include in your data package):  ([https://github.com/hex705/Glue](Glue) builds these packages quickly.)


	START_BYTE dataZero DELIMITER dataOne DELIMITER dataTwo DELIMITER END_BYTE



Assuming defaults of : START_BYTE = '*',  END_BYTE = '#",  DELIMITER =',' the package will be


	*dataZero,dataOne,dataTwo,#


(Yes, I know that a delimiter and an END_BYTE side-by-side looks odd -- but it simplifies).


Data ELEMENTS within the package are indexed 0-n (like an array).


### Include

Include the Scissors library.

		#include <Scissors.h>


The include statement can be added via menu:: Sketch --> Import Library --> Glue
###Instantiate

Scissors object must be declared at the  top of sketch:

	Scissors scissors;

### Defaults

	BAUD        19200
	START_BYTE  *
	END_BYTE    #
	DELIMITER   ,


### Begin -- in setup()

.begin() function must be called within  setup() :

	void setup() {

		scissors.begin( );  // will instantiate with defaults

	}

Note: At this time, the .begin( BAUD ) function starts the hardware serial port at BAUD rate.  Future versions will be more flexible with respect to STREAM source.  

#### Alternate constructors:

	scissors.begin( BAUD );  // sets Serial baud = BAUD

	scissors.begin( int, char, char, char ); // baud, startByte, endByte, delimiter


Debugging with serial communication ( eg. Serial.print() ) is possible once .begin() is called.

#### Setting Payload Parameters

	scissors.setStartByte( CHAR );
    scissors.setEndByte  ( CHAR );
    scissors.setDelimiter( CHAR );   


#### Getting Payload Parameters

	char sb = scissors.getStartByte();
	char eb = scissors.getEndByte  ();
	char de = scissors.getDelimiter();


#### Payloads without DELIMITERS
Some devices send payloads with START\_BYTEs and END\_BYTEs but no DELIMITERS (single element payloads).
RFID is an example of this -- to have SCISSORS return these PAYLOADs set delimiter to a negative value in your setup(){} routine.

	 scissors.setDelimiter( -1 );   // will properly parse payloads with NO delimiters

### Update -- in loop()

Data is retrieved from the buffer within loop() with a call to:

	int countElements = scissors.update();

Returns the number (int) of data points found in current package.  This could go into a function called from loop.


### Extract Data Points

Incoming serial data is stored in a string buffer that is automatically parsed at delimiters upon receipt.  Individual values can be obtained by by calling the following get functions:

	scissors.getInt( index );		// returns an int
	scissors.getFloat( index );    // returns a float
	scissors.getString( index );   // returns a String (Arduino String not a char[] )

where index = data position in original data package (zero indexed)

### Example Code for loop or function

	if (scissors.update() > 0) { // have a new message?

        int on = scissors.getInt(0);  // get int ELEMENT from MESSAGE index 0
        int off= scissors.getInt(1);  // get int ELEMENT from MESSAGE index 1   
  	}

### RAW Data

You can retrieve the whole data stream with:

	scissors.getRaw();		// returns Serial buffer up to END_BYTE

NOTE: ASCII characters less than 32 ( eg. 10 and 13 ) will be retrieved but will not show up in the Serial window.

### Package Size

Data points can be any combination of  int, float or String.  By default max number of data ELEMENTS in a single message is 8 (to limit memory tied up by library).  This can be reset with:


	int setMaxElements( int ) ;



## HISTORY
=======
### July / August 2023
-- reworking API to enable events (similar to OSC and MQTT).  In the early days of this project I didn't know how to set up an event call -- so the original interface used *.listen()* to read meassages manually -- usually in loop.  *.Listen()* is used in OSC to poll the connected **Stream**.  MQTT libraries tend to use *.poll()* for this function.  So in other libraries *.poll()* AND *.listen()* trigger events.   This creates consistency issues (for me and users).  After much going in circles I have decided to leave my *.listen()* in its original (problematic?) form as this gives better backwards compatability.  I am adding events to a *.poll()* function -- that is expected to be called in loop. 
-- adding a basic way to return Stream buffer -- it is going to be limited -- if you need this to be a robust ability then this library is probably wrong for you anyway. 
-- cleaning up Stream capture to be non-blocking.  


### January 2022 Arduino Scissors update
-- correct some error handling
-- serial stream is now passed to the library
-- debug can be turned on an off
-- delimiter before end byte no longer required, but kept for legacy
-- examples expanded -- more to come

### October 2012:
Added ability to retrieve messages without delimiters (eg RFID tags).
Added RFID example.
Code cleaning, readme updates.

### August, 2012:
First version uploaded to GITHUB

____



TO DO:
------

 * Further Examples

 * (completed 2022 - yup i am that fast) ~~tie to Stream not serial -- enabling use with Ethernet Shield, MQTT and other modes~~
		~~Note: this will change the instantiation and API~~

 * full documentation
