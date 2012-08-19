Puree ReadMe

Puree is a simple Hardware Serial parsing library for Arduino.

As of August 2012, Puree is an alpha release.  It is tested and appears stable.  Please report bugs here:

https://github.com/hex705/Puree/issues


Download:
Obtain archive (.zip) from   

		https://github.com/hex705/Puree

Unzip and copy folder (OSX) into:   ~/Documents/Arduino/libraries

Folder contains library, and examples.


Usage:


Puree expects structured messages with packages of the following form (spaces for clarity ONLY - do not include in your package):

START_BYTE dataZero DELIMITER dataOne DELIMITER dataTwo DELIMITER END_BYTE

Assuming default of : START_BYTE = '*',  END_BYTE = '#",  DELIMITER =','

*dataZero,dataOne,dataTwo,#

(Yes, I know that a delimiter and an END_BYTE side-by-side looks odd -- but it simplifies).

The example package has three data points indexed 0-2 (like an array).

Data points can be any combination of  int, float or String.  By default max number of data points in a single message is 8.  The can be reset with:

int setMaxElements( int ) ;


Puree object must be declared at the  top of sketch:

Puree puree;


.begin() function must be called within  setup() :

void setup() {

	puree.begin();  // will instantiate with defaults

}

Alternate constructors:

	puree.begin( int );  // set baud

	puree.begin( int, char, char, char ); // baud, startByte, endByte, delimiter

Defaults:

BAUD: 19200
START_BYTE =  '*' 
END_BYTE     =  '#'
DELIMITER    =   ','

Debugging with serial communication is possible once .begin() is called.

Data Stream:


Data is retrieved from the buffer within loop() with a call to:

puree.update();

Returns the number (int) of data points found in current package.


Extracting Data Points

Incoming serial data is stored in a string buffer that is automatically parsed at delimiters upon receipt.  Individual values can be obtained by by calling the following get functions:

int       puree.getInt( index );
float    puree.getFloat (index);
String puree.getString (index );

where index = data position in original data package (zero indexed)

RAW Data

You can retrieve the whole data stream with:

String  puree.getRaw();




TO DO:

 -- Further Examples
 -- tie to Stream not serial -- enabling use with Ethernet Shield
		Note: this will change the instantiation.

 -- full documentation to be developed.



