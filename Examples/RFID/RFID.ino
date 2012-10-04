
//  Example:: Scissors as an RFID tag reader 
//  Scissors can be used with RFID tag readers.

//  Match the BAUD in .begin() call to the BAUD of your reader.
//  Set DELIMITER to -1
//  Set START_BYTE and END_BYTE to match tag structure.

// Example developed by hex705 (Steve Daniels)
// github.com/hex705



#include <Scissors.h>


  Scissors RFID_reader;
  
  String newTag = "";
  String oldTag ="";
  
  
void setup() {
  
    RFID_reader.begin(2400); 
   
   // scissors needs to have the package structure set to match RFID tags.
   // each tag send a message of form:
   // START_BYTE payload_of_length_10_bytes END_BYTE
   
     RFID_reader.setStartByte( 0x0A ); // 10 == start_byte
     RFID_reader.setEndByte  ( 0x0D ); // 13 == end_byte
     RFID_reader.setDelimiter( -1 );   // no delimiters 
   
   // all of the above could be replaced with the following
   // RFID_reader.begin(2400,10,13,-1); 
  
} 



void loop () {

   if ( RFID_reader.update() > 0 ) {         // poll the SCISSOR object -- any new MESSAGES (returns element count)
        
        
       newTag = RFID_reader.getString(0);       // get ELEMENT 2 from MESSAGE -- assuming ELEMENT(2) is a  float     
       
       if ( ! newTag.equals(oldTag) ) {
           Serial.println(newTag);                 // only print new tags that are different than last tag  
       }
       
       oldTag = newTag;

   }
   
   
   
  delay(100); // 2400 BAUD is fairly slow -- a little delay helps keep the system from bogging down
  
} 


