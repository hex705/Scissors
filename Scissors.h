//
//  Scissors.h
//
//
//  Created by steve daniels on 12-08-19.
//  Copyright (c) 2012 __ribosome.ca__. All rights reserved.
//


#ifndef Scissors_h
#define Scissors_h


#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Stream.h>
#include <WString.h>        // String
#include <HardwareSerial.h>

#define LOCAL 0

class Scissors
{

public:

   String messageBuffer ;
   String sub ;

   //constructor
   Scissors();

   // setup functions
   void begin();
   void begin(char, char, char);
   void begin( Stream & );
   void begin( Stream &, char, char, char );
   // testing
   void sendText(String);

	// parse functions
	int    listen    ( );
	int    parse   ( String );
	int errorCheck();

	int    getInt    ( int );
	float  getFloat  ( int );
	String getString ( int );
	String getRaw    ( );

	// setters
	int setStartByte( char );
	int setEndByte  ( char );
	int setDelimiter( char );
	int setMaxElements( int ) ;

	// getters
	char getStartByte   ( );
	char getEndByte     ( );
	char getDelimiter   ( );
	int  getMaxElements ( );
	int  getBaud        ( );

  // debug levels
  int VERBOSE = 0;
  int setDebug (int);

private:

  void init (char,char,char);

  Stream *theStream;
	// internal vars & parsing variables
	char START_BYTE = '*';
	char END_BYTE ='#' ;
	char DELIMITER= ',';
	int  MAX_ELEMENTS=8;
	int  BAUD ;

	int elementCount ; //=  0;
	int messageStart ; // = -1;
	int messageEnd   ; // = -1;
	int delims[8]   ; //   [MAX_ELEMENTS+1]   array to hold delimiter locations in buffer string

   // create version where we pass a string for parsing
    boolean useSerial=true; // turn off serial when 1 passed as BAUD -- [jan2022 1 or 0? LOCAL defined as 0 above]


	// buffers for incoming data ( yes they could be a char[] -- but they arn't )


	void init (int, char, char, char);
	int findDelimiters();
	String getElement ( int ) ; // not called directly by user



}; //endclass Scissors


#endif
