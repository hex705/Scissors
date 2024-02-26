// Scissors Example:  listener basic
// manually check for message and parse. 

//https://github.com/hex705/Scissors


/* paste entire line below into serial monitor once uploaded
   *123,45.678,EOM,#
*/

#include <Scissors.h>
Scissors cut;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  cut.begin(Serial);

  // info messages
  Serial.println("waiting for message");
  Serial.println("paste following into monitor above");
  Serial.println("*123,45.678,EOM,#");
}

void loop() {

  if (cut.listen() > 0) // manually check for messages 
  {
      //data extraction from incoming message
      int    i = cut.getInt(0);
      float  f = cut.getFloat(1);
      String s = cut.getString(2);

      // you could put this in a function
      Serial.println("\nReceived");
      Serial.println(cut.getRaw()); // raw message - alt way
      Serial.println(i); // int
      Serial.println(f); // float
      Serial.println(s); // string
      Serial.println();
  }

  // other stuff 

}
