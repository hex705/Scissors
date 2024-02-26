//  Scissors Example:  RFID.ino
//  Scissors used as an RFID tag reader 
//  updated 2024 (hex705)

//  Match the BAUD in .begin() call to the BAUD of your reader.
//  Set DELIMITER to -1
//  Set START_BYTE and END_BYTE to match tag structure.

/* circuit 
   vcc= 5V
   gnd = gnd
   s-out of reader to pin 0 (RX arduino UNO )
   sn (reader) is enable it must be LOW -- connect  to ground or control with pin (in this case 2)
*/

/* 
   indicators
   ironically -- a red LED is good on my reader -- a green led is bad
   -- when the enable pin is pulled LOW (down) teh LED goes red adn tags can be read
*/


// ************   NOTE -- upload challenges   **************
// in this care RFID is using UNO serial pins.
// these are connected to USB 
// !!!   you MUST disconnect the data line to UNO pin 0 to upload, then reconnect to read  !!!!
// ************


// Example developed by hex705 (Steve Daniels)
// github.com/hex705


#include <Scissors.h>


  Scissors RFID_reader;
  
  String newTag = "";
  String oldTag ="";

  int enablePin = 2;
  
  
void setup() {
    Serial.begin(2400);
    RFID_reader.begin(Serial); 
   
   // scissors needs to have the package structure set to match RFID tags.
   // each tag send a message of form:
   // START_BYTE payload_of_length_10_bytes END_BYTE
   
     RFID_reader.setStartByte( 0x0A ); // 10 == start_byte
     RFID_reader.setEndByte  ( 0x0D ); // 13 == end_byte
     RFID_reader.setDelimiter( -1 );   // no delimiters 
   
   // all of the above could be replaced with the following
   // RFID_reader.begin(2400,10,13,-1); 

  pinMode(enablePin,OUTPUT);
  int enablePin = 2;
  digitalWrite(enablePin,LOW); // enable LOW ! 
  
} 


void loop () {

  RFID_reader.poll();  // check for messages

  if ( ! newTag.equals(oldTag) ) {
      Serial.println(newTag);                 // only print new tags that are different than last tag  
  } 
       
  oldTag = newTag;
       
  delay(100); // 2400 BAUD is fairly slow -- a little delay helps keep the system from bogging down
  
} 

void scissorsEvent( String &theMessage ){  // its just a string

    Serial.println("\nReceived ");
    Serial.println(RFID_reader.getRaw());

    //data extraction from incoming message
    Serial.println("parsing message");
    newTag = RFID_reader.getString(0);

}
