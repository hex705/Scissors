// Scissors Example:  parseBasic.ino
// parse splits a source string that conforms to pattern:

// *data0,data1,data2,#

//https://github.com/hex705/Scissors


/* paste entire line below into serial monitor once uploaded
   *123,45.678,EOM,#
*/

#include <Scissors.h>
Scissors scissors;

String s = "*123,45.678,EOM,#";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  scissors.begin();  // NO STREAM
  Serial.println(__FILE__);
  Serial.println();
}

void loop() {

    Serial.println("parsing internal message:");
    Serial.println(s);

    // parse and extract data from message when called 
    scissors.parse(s); // parse insternal string (NO STREAM)

    // parse only splits the message -- you still need to extract the data
    int    pos0 = scissors.getInt(0);
    float  pos1 = scissors.getFloat(1);
    String pos2 = scissors.getString(2);

    // debug the result -- comment out if desired 
    Serial.println("\nRaw message: ");
    Serial.println(scissors.getRaw()); // raw message - alt way
    Serial.println();

    Serial.println("parsed data: ");

    Serial.println(pos0); // int
    Serial.println(pos1); // float
    Serial.println(pos2); // string

    // your other stuff
    // ... 



   while(1); // blocks - ensures loop only runs once for this demo.
}
