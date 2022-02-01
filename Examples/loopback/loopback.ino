// scissors and glue loopback
// recieves a message from serial, and sends it back reformatted.
// needs scissors and glue

//https://github.com/hex705/Glue

//https://github.com/hex705/Scissors


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
  Serial.println("waiting for message");
  Serial.println("paste following into monitor above");
  Serial.println("*123,45.678,EOM,#");
}

void loop() {
  // wait for message 
  if(cut.update()>0){
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
    Serial.println("Glue repackaged to :: ");
    elmers.create();
    elmers.add(s);
    elmers.add(f);
    elmers.add(i);
    elmers.send(); 
  }

}
