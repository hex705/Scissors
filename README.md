Scissors  (ReadMe)
=====  

*Scissors* is a simple Hardware Serial parsing library for Arduino.

*Scissors* plays well with its sibling library *Glue*.  Glue and Scissors are also available for Processing.

As of August 2012, Scissors is an alpha release.  It is tested and appears stable.  Please report bugs here:

[https://github.com/hex705/Scissors/issues](https://github.com/hex705/Scissors/issues "Issues")

January 2022 Arduino Scissors update
-- correct some error handling
-- serial is now passed to the library
-- debug can be turned on an off
-- delimiter before end byte no longer required, but kept for legacy
-- examples expanded -- more to come


Quick Start
---------------

1. Obtain archive (.zip) @  [https://github.com/hex705/Scissors](https://github.com/hex705/Scissors "Download")

2. Unzip and (if needed) rename folder Scissors

3. Copy folder (OSX) into:   ~/Documents/Arduino/libraries

4. Start cutting data streams.

Downloaded folder contains library, and examples.


Usage
-----

Scissors expects structured messages with packages of the following form (spaces for clarity ONLY - do not include in your package):  ([https://github.com/hex705/Glue](Glue) builds these packages quickly.)


	START_BYTE dataZero DELIMITER dataOne DELIMITER dataTwo DELIMITER END_BYTE



Assuming defaults of : START_BYTE = '*',  END_BYTE = '#",  DELIMITER =',' the package will be


	*dataZero,dataOne,dataTwo,#


(Yes, I know that a delimiter and an END_BYTE side-by-side looks odd -- but it simplifies).


Data ELEMENTS within the package are indexed 0-2 (like an array).


### Include

Include the Scissors library.

		#include <Scissors.h>


The include statement can be added via menu:: Sketch --> Import Library --> Glue
###Instantiate

Scissors object must be declared at the  top of sketch:

	Scissors scissors;

###Defaults

	BAUD        19200
	START_BYTE  *
	END_BYTE    #
	DELIMITER   ,


###Begin -- in setup()

.begin() function must be called within  setup() :

	void setup() {

		scissors.begin( );  // will instantiate with defaults

	}

Note: At this time, the .begin( BAUD ) function starts the hardware serial port at BAUD rate.  Future versions will be more flexible with respect to STREAM source.  

####Alternate constructors:

	scissors.begin( BAUD );  // sets Serial baud = BAUD

	scissors.begin( int, char, char, char ); // baud, startByte, endByte, delimiter


Debugging with serial communication ( eg. Serial.print() ) is possible once .begin() is called.

####Setting Payload Parameters

	scissors.setStartByte( CHAR );
    scissors.setEndByte  ( CHAR );
    scissors.setDelimiter( CHAR );   


####Getting Payload Parameters

	char sb = scissors.getStartByte();
	char eb = scissors.getEndByte  ();
	char de = scissors.getDelimiter();


####Payloads without DELIMITERS
Some devices send payloads with START\_BYTEs and END\_BYTEs but no DELIMITERS (single element payloads).
RFID is an example of this -- to have SCISSORS return these PAYLOADs set delimiter to a negative value in your setup(){} routine.

	 scissors.setDelimiter( -1 );   // will properly parse payloads with NO delimiters

###Update -- in loop()

Data is retrieved from the buffer within loop() with a call to:

	int countElements = scissors.update();

Returns the number (int) of data points found in current package.  This could go into a function called from loop.


###Extract Data Points

Incoming serial data is stored in a string buffer that is automatically parsed at delimiters upon receipt.  Individual values can be obtained by by calling the following get functions:

	scissors.getInt( index );		// returns an int
	scissors.getFloat( index );    // returns a float
	scissors.getString( index );   // returns a String (Arduino String not a char[] )

where index = data position in original data package (zero indexed)

###Example Code for loop or function

	if (scissors.update() > 0) { // have a new message?

        int on = scissors.getInt(0);  // get int ELEMENT from MESSAGE index 0
        int off= scissors.getInt(1);  // get int ELEMENT from MESSAGE index 1   
  	}

###RAW Data

You can retrieve the whole data stream with:

	scissors.getRaw();		// returns Serial buffer up to END_BYTE

NOTE: ASCII characters less than 32 ( eg. 10 and 13 ) will be retrieved but will not show up in the Serial window.

###Package Size

Data points can be any combination of  int, float or String.  By default max number of data ELEMENTS in a single message is 8 (to limit memory tied up by library).  This can be reset with:


	int setMaxElements( int ) ;



HISTORY
=======

##October 2012:
Added ability to retrieve messages without delimiters (eg RFID tags).
Added RFID example.
Code cleaning, readme updates.

##August, 2012:
First version uploaded to GITHUB



____



TO DO:
------

 * Further Examples

 * tie to Stream not serial -- enabling use with Ethernet Shield
		Note: this will change the instantiation and API

 * full documentation
