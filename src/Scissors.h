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

class Scissors
{

public:

   String messageBuffer ;
   String sub ;

   //constructor
   Scissors();

   // setup functions
   // no stream - data is local - or from other source
   void begin();
   void begin( char, char, char );

   // from data stream
   void begin( Stream & );
   void begin( Stream &, char, char, char );
   
   // testing
   void sendText(String);

	// receive and parse functions
	int parse( String );
	int listen( );
	void poll(); 
		// int available();
		// int read();

    // functions for extracting data - int = position in message
	int    getInt    ( int );
	float  getFloat  ( int );
	String getString ( int ); 
	String getRaw    ( );  // unparsed raw message

	// functions for gettting message structure info
	int getMessageLength();
	int getTokenCount();

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
	int DEBUG = 0; // 0 = off, 1 == verbose, 2 = verbose + fxn names
	int setDebug (int);

private:

  void init (char,char,char);

  String scissorsMessage;

  Stream *theStream;

	// internal vars & parsing variables
	char START_BYTE = '*';
	char END_BYTE ='#' ;
	char DELIMITER= ',';
	int  MAX_ELEMENTS=8;  // you change this - chaenge next too -- i need a better solution
	static const int DEFAULT_ELEMENTS = 8; 
	int  BAUD ;

	int messageStart ; // = -1;
	int messageEnd   ; // = -1;
	int delims[DEFAULT_ELEMENTS+1]   ; //[MAX_ELEMENTS+1]   array to hold delimiter locations in buffer string

    // create version where we pass a string for parsing
    boolean useStream=true; // do not start serial if stream is left out -- [jan2022 1 or 0? LOCAL defined as 0 above]

	// buffers for incoming data ( yes they could be a char[] -- but they arn't )
	void init (int, char, char, char);

    int readStream();
	int findDelimiters();
	int getTokens();
	int errorCheck();
	String getElement ( int ) ; // not called directly by user

	// july 2023 -- rework
	// message properties -- all global now -- not really needed this way. 
	int elementCount ; //=  0;
	int messageLength;

	// messageFlags -- meeded ?? 
	int messageArrived;
	int messageOk;
	int messageParsed;

}; //endclass Scissors

#endif
