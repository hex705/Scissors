Scissors  (ReadMe)
=====  

*Scissors* is a simple Hardware Serial parsing library for Arduino.

As of August 2012, Scissors is an alpha release.  It is tested and appears stable.  Please report bugs here:

[https://github.com/hex705/Scissors/issues](https://github.com/hex705/Scissors/issues "Issues")



Quick Start
---------------

1. Obtain archive (.zip) @  [https://github.com/hex705/Scissors](https://github.com/hex705/Scissors "Download")

3. Unzip and copy folder (OSX) into:   ~/Documents/Arduino/libraries

4. Start cutting data streams.

Downloaded folder contains library, and examples.


Usage
-----

Scissors expects structured messages with packages of the following form (spaces for clarity ONLY - do not include in your package):


	START_BYTE dataZero DELIMITER dataOne DELIMITER dataTwo DELIMITER END_BYTE



Assuming defaults of : START_BYTE = '*',  END_BYTE = '#",  DELIMITER =',' the package will be


	*dataZero,dataOne,dataTwo,#


(Yes, I know that a delimiter and an END_BYTE side-by-side looks odd -- but it simplifies).


The example package has three data points indexed 0-2 (like an array).

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

		scissors.begin();  // will instantiate with defaults

	}

####Alternate constructors:

	scissors.begin( int );  // set baud

	scissors.begin( int, char, char, char ); // baud, startByte, endByte, delimiter


Debugging with serial communication ( eg. Serial.print() ) is possible once .begin() is called.


###Update -- in loop()

Data is retrieved from the buffer within loop() with a call to:

	scissors.update();

Returns the number (int) of data points found in current package.


###Extract Data Points

Incoming serial data is stored in a string buffer that is automatically parsed at delimiters upon receipt.  Individual values can be obtained by by calling the following get functions:

	scissors.getInt( index );		// returns an int
	scissors.getFloat( index );    // returns a float
	scissors.getString( index );   // returns a String (Arduino String not a char[] )

where index = data position in original data package (zero indexed)

###RAW Data

You can retrieve the whole data stream with:

	scissors.getRaw();		// returns Serial buffer up to END_BYTE


###Package Size

Data points can be any combination of  int, float or String.  By default max number of data points in a single message is 8.  The can be reset with:


	int setMaxElements( int ) ;




TO DO:
------

 * Further Examples

 * tie to Stream not serial -- enabling use with Ethernet Shield
		Note: this will change the instantiation and API

 * full documentation 



