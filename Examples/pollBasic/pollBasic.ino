// listener basic
// receives message and triggers scissorsEvent

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

  cut.setDebug(1); 

  // info messages
  Serial.println("waiting for message");
  Serial.println("paste following into monitor above");
  Serial.println("*123,45.678,EOM,#");
}

void loop() {
    // other stuff

    cut.poll(); // triggered automatically when message has arrived
}

void scissorsEvent( String &theMessage ){  // its just a string

    Serial.println("\nReceived ");
    Serial.println(theMessage);

    //data extraction from incoming message
    Serial.println("parsing message");
    int i0 = cut.getInt(0);
    int i1 = cut.getInt(1);
    int i2 = cut.getInt(2);
    int i3 = cut.getInt(3);
    int i4 = cut.getInt(4);
    int i5 = cut.getInt(5);
    int i6 = cut.getInt(6);
    int i7 = cut.getInt(7);
    int i8 = cut.getInt(8);
    int i9 = cut.getInt(9);

    // you could put following in a function

    // Serial.println("\nReceived (raw) ");
    // Serial.println(cut.getRaw()); // raw message - alt way

    Serial.println("data received");

    Serial.println(i0); // int
    Serial.println(i1); // float
    Serial.println(i2); // string
    Serial.println(i3); // float
    Serial.println(i4); // string
    Serial.println(i5); // string
    Serial.println(i6); // string
    Serial.println(i7); // string
    Serial.println(i8); // string
    Serial.println(i9);
    Serial.println();
}
