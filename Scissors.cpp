//
//  Scissors.cpp
//  
//
//  Created by steve daniels on 12-08-19.
//  Copyright (c) 2012 __ribosome.ca__. All rights reserved.
//


#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Scissors.h>
#include <WString.h> // String
// Scissors#include <HardwareSerial.h>


void Scissors::begin()
{
	init( 19200, '*', '#', ',' ); // default setup
}

void Scissors::begin( int _baud )
{
	init( _baud, '*', '#', ',' ); // specified baud, default parse parameters
}

void Scissors::begin( int _baud, char _start_byte, char _end_byte, char _delimiter )
{
	init( _baud,  _start_byte,  _end_byte,  _delimiter); // user specifies all terms
}


void Scissors::init( int _baud, char _start_byte, char _end_byte, char _delimiter)
{
		
	BAUD = _baud;
	START_BYTE = _start_byte;
	END_BYTE   = _end_byte;
	DELIMITER  = _delimiter;
	MAX_ELEMENTS = 8;
	
	elementCount =  0;
	messageStart = -1;
	messageEnd   = -1;
	delims[MAX_ELEMENTS+1]        ; //   [MAX_ELEMENTS+1]   array to hold delimiter locations in buffer string
	
	//String messageBuffer = "" ; 
	//String sub ="";
	
	Serial.end();
	delay(5);
	Serial.begin(BAUD);
	
}

int Scissors::update() {
  
  int state = 0;
 
  if (Serial.available() ){
    
    messageBuffer =""; // clear the buffer
    messageStart = -1;
    messageEnd   = -1;
    
    delay(10);  // for reasons I don't understand this is really important -- do not remove
    
	// go get all the characters in the buffer
    while(Serial.available() > 0) { 
      
      char c = Serial.read();
      messageBuffer += c;
        
    }
    
   // a bit of error checking
   int s = messageBuffer.indexOf(START_BYTE);
   //int e = messageBuffer.indexOf(END_BYTE);
   int e = messageBuffer.lastIndexOf(DELIMITER);
			//	Serial.print("e is ");
			//	Serial.println(e);
   if (e <= 0) { // did not find a delimiter
	
		// so, check for an end byte, some messages do not have DELIMITERS
		  e = messageBuffer.indexOf(END_BYTE);
			//	Serial.print("e could be ");
			//	Serial.println(e);
	}
	
   if (s >= 0 ) { 
 
     messageStart = s;

     if (e > s)  {	   
         messageEnd = e;
        // appear to have a whole message -- reset parse values
      // messageEnd = messageBuffer.lastIndexOf(DELIMITER);
       // memset( delims, 0, ( sizeof(delims) / sizeof(delims[0]) ) ); // clear delims array for new values
      
       state = findDelimiters();

       Serial.flush();
       delay(1);

       
     } // end e>s
     else {
      state = -2; // no end found 
     }
     
   } // end s>=0
   else {
     state = -1; // no start  
   }
      
  } // if serial.available
  Serial.flush();
  return state;
  
}


int Scissors::findDelimiters() {
  
    // delims array stores locations of DELIMITERS -- there 
          elementCount = 0;
          delims[elementCount] = messageStart;     
             
          for (int i = messageStart+1; i <= messageEnd;  i ++) {  
            
            if (elementCount > MAX_ELEMENTS) break; // message is longer than max -- overflow
                                                     // this overflow is not YET signalled
            if ( (messageBuffer.charAt(i) == DELIMITER)   ) { 
                  
                  elementCount ++; 
                  delims[elementCount] = i ;
                  if (messageBuffer.charAt(i+1) == END_BYTE) break;
                  
            } // end if         
            
			
          } // end for 

		// we are at end of message -- if elementCount still == 0 then there were no delimiters
		// however, we can only be here if we had an end byte after a start byte -- 
		// SO, there must be a single undelimited ELEMENT in the message 
		if ( elementCount == 0 ) {
		//	Serial.println("no delims");
			elementCount = 1;
			delims[1] = messageEnd;
		}
		
   
   return elementCount;

}  // end parser


String Scissors::getElement ( int whichOne ){  // not called directly by user

  sub ="";
  if ( (whichOne >= 0) && (whichOne <= elementCount) ) 
  {
    
      int startLoc = delims[whichOne]+1 ;
      int endLoc   = delims[whichOne+1] ;
      
      if ( startLoc > endLoc ) // START_BYTE comes after END_BYTE
      {  
	
         sub = "err: Sb4E";   // start before end
          
      } 
      else {  // START_BYTE preceeds END_BYTE
        
         for (int c= startLoc ;  c < endLoc ; c++ ) 
         {
         	sub += messageBuffer.charAt(c);
         }
        
      }
 
     
  } 
  else {
    
  	sub = "err: ElOoR"; // element out of range

  }

   return sub;
  
} // end getElement



int Scissors::getInt ( int whichOne ){
    
    // http://www.cplusplus.com/reference/clibrary/cstdlib/atoi/
    // no standard way to solve issue of STRING input not being in range of INT
    // this will fail QUITELY!!!!

  
  String elementString = getElement(whichOne);
  
  char char_buff[elementString.length() + 1];
  
  elementString.toCharArray(char_buff, sizeof(char_buff));
  if (char_buff[0] <'0' || char_buff[0] > '9') {
	Serial.println("NaN");	
  }
  return atoi(char_buff);

}



float Scissors::getFloat( int whichOne) {
  
  String elementString = getElement(whichOne);
  
  char char_buff[elementString.length() + 1];
  elementString.toCharArray(char_buff, sizeof(char_buff));

  if (char_buff[0] <'0' || char_buff[0] > '9') {
	Serial.println("NaN");	
  }
  
  return atof(char_buff); 

}



String Scissors::getString (int whichOne) {
  return getElement(whichOne);
}


String Scissors::getRaw() {
   return messageBuffer; 
}




int Scissors::setStartByte(char s) {
  START_BYTE = s;
  return 1;
}


int Scissors::setEndByte(char e) {
  END_BYTE = e;
  return 1;
  
}


int Scissors::setDelimiter(char d) {
  DELIMITER = d;
  return 1;
  
}

int Scissors::setMaxElements( int m ) {
	MAX_ELEMENTS = m;
	delims[MAX_ELEMENTS+1];    
	return 1; 
}


char Scissors::getStartByte( ) {
   return START_BYTE;
}


char Scissors::getEndByte( ) {
   return END_BYTE;
}


char Scissors::getDelimiter( ) {
   return DELIMITER;
}


int Scissors::getMaxElements(  ) { 
	return 	MAX_ELEMENTS ; 
}

int Scissors::getBaud(){
	
	return BAUD ;
}






