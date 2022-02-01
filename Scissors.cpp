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

Scissors::Scissors() {
	;
}

void Scissors::begin(){
	useSerial = false;
	init('*','#',',');
}

void Scissors::begin(char _start_byte, char _end_byte, char _delimiter ){
	useSerial = false;
	init( _start_byte,  _end_byte,  _delimiter);
}

void Scissors::begin(Stream &s){
	theStream = &s;
	init('*','#',',');
}
void Scissors::begin(Stream &s, char _start_byte, char _end_byte, char _delimiter){
	theStream = &s;
	init( _start_byte,  _end_byte,  _delimiter);
}

void Scissors::sendText(String someText){
	theStream->println(someText);
}

// void Scissors::begin( int _baud )
// {
// 	if ( _baud == LOCAL ) { // baud of zero no serial
// 		useSerial = false;
// 	} else {
// 		useSerial = true;
// 	}
//
// 	init( _baud, '*', '#', ',' ); // specified baud, default parse parameters
//
// }

// void Scissors::begin( Stream &s, char _start_byte, char _end_byte, char _delimiter )
// {
// 	stream = new Serial(s);
// 	init( _start_byte,  _end_byte,  _delimiter); // user specifies all terms
// }


//

void Scissors::init( char _start_byte, char _end_byte, char _delimiter)
{

	START_BYTE = _start_byte;
	END_BYTE   = _end_byte;
	DELIMITER  = _delimiter;
	MAX_ELEMENTS = 8;

	elementCount =  0;
	messageStart = -1;
	messageEnd   = -1;
	delims[MAX_ELEMENTS+1]; //   [MAX_ELEMENTS+1]   array to hold delimiter locations in buffer string
}

// check buffer for data
int Scissors::update() {

	int state = -99;

	if ( useSerial ) {

		if ( theStream->available() ){

			messageBuffer =""; // clear the buffer
			messageStart = -1;
			messageEnd   = -1;

			delay(10);  // for reasons I don't understand this is really important -- do not remove

			// J22 -- this must be blocking ? check ?
			// go get all the characters in the buffer
			while(theStream->available() > 0) {

				char c = theStream->read(); // read char
				messageBuffer += c; // store it in string
				delay(1);
			}

			state = errorCheck();

		} // if serial.available

		theStream->flush();
	}  // end useSerial
	return state;

}

int Scissors::update(String inString) {

	int state = 0;
	messageBuffer = inString;
	errorCheck();
	//Serial.print("MB");
	// if (VERBOSE==1) Serial.println(messageBuffer);
	return state;

}

// [j22 - fix this first]
int  Scissors::errorCheck() {
	int state = -1; // number of delimiters found
	int status = 6; // parse status, 1 = success

	messageStart = -1;
	messageEnd   = -1;

   if (VERBOSE==1) Serial.println(" got message in");
	 if (VERBOSE==1) Serial.println(messageBuffer);

	// a bit of error checking
	int s = messageBuffer.indexOf(START_BYTE);
	int e = messageBuffer.indexOf(END_BYTE);
	int d = messageBuffer.lastIndexOf(DELIMITER);

	if (s == -1) {
		status = 5; // no start
		 if (VERBOSE==1) Serial.println("no start");
	} else if (e == -1) {
		status = 4; // no end
		 if (VERBOSE==1) Serial.println("no end");
	} else if (s<e) {
		//havestart and end - in correct order
		status = 1; // potential message
		 if (VERBOSE==1) Serial.println("start and end ok");

		// check for empty, s and e side by side
		if ( s == e-1){
			status = 2;
			 if (VERBOSE==1) Serial.println("empty message");
		}   //  s==e-1

		// check if no DELIMITERS - may only have end byte
		if (d <= 0){
			 if (VERBOSE==1) Serial.println("no delims - message with one token");
			// set end of message to end byte
			e = messageBuffer.indexOf(END_BYTE);
		} // d<=0

		//check for delim beside end
		// if true make end = delim, not end byte
		if (d == e-1) {
			 if (VERBOSE==1) Serial.println("comma before end, what to do ?");
			e = d;
		}

		// ok to parse - have message
		if  (status == 1) {
			messageStart = s;
			messageEnd = e;

			state = findDelimiters();

			 if (VERBOSE==1)
			 Serial.print("state = "); Serial.println(state);
		}

		delay(1);
	} // end else if s<e

	return state;
}



int Scissors::findDelimiters() {

	// delims array stores locations of DELIMITERS -- there
	elementCount = 0;
	delims[elementCount] = messageStart;

	for (int i = messageStart+1; i <= messageEnd;  i ++) {

		if (elementCount > MAX_ELEMENTS) break; // message is longer than max -- overflow
		// this overflow is not YET signalled
		if ( (messageBuffer.charAt(i) == DELIMITER)||(messageBuffer.charAt(i) == END_BYTE)   ) {

			elementCount ++;
			delims[elementCount] = i ;
			if (messageBuffer.charAt(i+1) == END_BYTE) break;

		} // end if

	} // end for

	 if (VERBOSE==1) {
		 Serial.println("delims");
		 for (int i: delims)   {Serial.print(i);Serial.print(' ');}
		 Serial.println();
   }

	// we are at end of message -- if elementCount still == 0 then there were no delimiters
	// however, we can only be here if we had an end byte after a start byte --
	// SO, there must be a single undelimited ELEMENT in the message
	if ( elementCount == 0 ) {
		//	 if (VERBOSE==1) Serial.println("no delims");
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

			for (int c = startLoc ;  c < endLoc ; c++ )
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

	int lowIndex = 0;

	String elementString = getElement(whichOne);

	char char_buff[elementString.length() + 1];

	elementString.toCharArray(char_buff, sizeof(char_buff));

	// check for neg -- without this we reject neg values
	if ( char_buff[0] == '-' )  {
		lowIndex = 1;
		// if (VERBOSE==1) Serial.println("neg val");
	}

	if ( ( char_buff[lowIndex] <'0' || char_buff[0] > '9') ) {
		 if (VERBOSE==1) Serial.println("NaN char_buff next :: ");
		Serial.print(char_buff);
		return -9999;
	}

	return atoi(char_buff);

}



float Scissors::getFloat( int whichOne) {

	int lowIndex = 0;

	String elementString = getElement(whichOne);

	char char_buff[elementString.length() + 1];

	elementString.toCharArray(char_buff, sizeof(char_buff));

	// check for neg -- without this we reject neg values
	if ( char_buff[0] == '-' )  {
		lowIndex = 1;
		delay(10);
	}

	if (char_buff[lowIndex] <'0' || char_buff[0] > '9') {
		 if (VERBOSE==1) Serial.println("NaN");
		return -9999;
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

int Scissors::setDebug(int debugLevel){
	VERBOSE = debugLevel;
	return 1;
}
