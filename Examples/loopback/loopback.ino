// Scissors Example:  scissors and glue loopback

// recieves a message from serial, and sends it back reformatted.
// needs scissors and glue

//https://github.com/hex705/Glue

//https://github.com/hex705/Scissors


// this example uses the .listen() interface of scissors
// it could be done with .poll() as well. 

/* paste entire line below into serial monitor once uploaded
   *123,45.678,EOM,#
*/

#include <Scissors.h>
#include <Glue.h>

Scissors cut;
Glue elmers;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  cut.begin(Serial);
  elmers.begin(Serial);

  Serial.println(__FILE__);
  Serial.println("Waiting for message");
  Serial.println("Paste following into Serial monitor");
  Serial.println("*123,45.678,EOM,#");
}

void loop() {
  // wait for message
  if(cut.listen()>0){
    // have a message unpack it
    int    i = cut.getInt(0);
    float  f = cut.getFloat(1);
    String s = cut.getString(2);

    Serial.println("\nReceived");
    Serial.println(cut.getRaw()); // raw message
    Serial.println(i); // int
    Serial.println(f); // float
    Serial.println(s); // string
    Serial.println();

    // repackage with glue
    // note flip order of variables to know we changed it
    Serial.println("Glue repackaged to :: ");
    elmers.create();
    elmers.add(s);
    elmers.add(f);
    elmers.add(i);
    elmers.send(); // adds end byte and prints to monitor
  }

}
