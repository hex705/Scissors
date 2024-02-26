// Scissors Example: pollBasic

// pollBasic uses Scissors library .poll() function to trigger scissorsEvent().
// Strings from serial (Arduino Stream) are used.

// this example demonstrates passing a message string with three values
// each value is a different type ( int, float, String)


// default message structure
// start     = '*'
// end       = '#'
// delimeter = ','
// message structure :: *data0,data1,data2#

// can parse up to 8 data tokens -- may get slower at that level (YMMV)

// Scissors and this Example by hex705 (Steve Daniels)

// January 2012, All Rights Reserved.
// updated Jan 2022, Jan 2024

// Scissors and similar Libraries can be found at:
// https://github.com/hex705
// this library
// https://github.com/hex705/Scissors


/* paste entire line below into serial monitor once uploaded
   *123,45.678,EOM,#
*/

#include <Scissors.h>
Scissors scissors;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  scissors.begin(Serial);  // begin with stream

  // info messages
  Serial.println(__FILE__); 
  Serial.println("Waiting for message!\n");
  Serial.println("Paste following into Serial monitor");
  Serial.println("*123,45.678,EOM,#");
}

void loop() {
    // your other stuff

    // poll triggers scissorsEvent when message arrives from stream
    scissors.poll(); // constantly check stream (in this case Serial)
}


void scissorsEvent( String &theMessage ){  // its just a string

    // show full message
    Serial.println("\nReceived ");
    Serial.println(theMessage);

    // data extraction from incoming message
    // make (pos) varaibles global to use outside event. 
    Serial.println("\nParsing message.");
    int    pos0 = scissors.getInt(0);
    float  pos1 = scissors.getFloat(1);
    String pos2 = scissors.getString(2);

    // debug the result-- comment out if desired 
    Serial.println("Raw message received: ");
    Serial.println(scissors.getRaw()); // raw message - alt way

    Serial.println("data received: ");

    Serial.println(pos0); // int
    Serial.println(pos1); // float
    Serial.println(pos2); // string
  
    Serial.println();

}
