//
//  Puree.h
//  
//
//  Created by steve daniels on 12-08-19.
//  Copyright (c) 2012 __ribosome.ca__. All rights reserved.
//


#ifndef Puree_h
#define Puree_h


#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Stream.h>
#include <WString.h>        // String
#include <HardwareSerial.h>

class Puree
{
	
public: 
	
	
   String messageBuffer ; 
   String sub ;
	
    // constructors
    void  begin();      // default baud
	void  begin( int ); // baud
	void  begin( int, char, char, char ); // baud, start,end, delim
	
	// parse functions
	int    update    ( );
	int    getInt    ( int );
	float  getFloat  ( int );
	String getString ( int );
	String getRaw    ( );
	
	// setters
	int setStartByte( char );
	int setEndByte  ( char );
	int setDelimiter( char );
	int setMaxElements( int ) ;
		

	
private:
	
	// internal vars & parsing variables
	char START_BYTE;
	char END_BYTE  ;
	char DELIMITER ;  
	int  MAX_ELEMENTS;
	int  BAUD ;
	
	int elementCount ; //=  0;
	int messageStart ; // = -1;
	int messageEnd   ; // = -1;
	int delims[8]   ; //   [MAX_ELEMENTS+1]   array to hold delimiter locations in buffer string
	
	// buffers for incoming data ( yes they could be a char[] -- but they arn't )

	
	void init (int, char, char, char);
	int findDelimiters();
	String getElement ( int ) ; // not called directly by user
	
	
	
}; //endclass Puree


#endif
