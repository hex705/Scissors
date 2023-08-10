//
//  Scissors.cpp
//
//  Created by steve daniels on 2012-08-19.
//  Copyright (c) 2012 __ribosome.ca__. All rights reserved.
//  updated aug 2022, and aug 2023
//

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Scissors.h>
#include <WString.h> // String

// Scissors#include <HardwareSerial.h>

// event prototype
void scissorsEvent(String &);

Scissors::Scissors()
{
	; // declare the empty object
}

// for using the library without a stream ??
void Scissors::begin()
{
	useStream = false;
	init('*', '#', ',');
}

void Scissors::begin(char _start_byte, char _end_byte, char _delimiter)
{
	useStream = false;
	init(_start_byte, _end_byte, _delimiter);
}

// with stream
void Scissors::begin(Stream &s)
{
	theStream = &s;
	init('*', '#', ',');
}
void Scissors::begin(Stream &s, char _start_byte, char _end_byte, char _delimiter)
{
	theStream = &s;
	init(_start_byte, _end_byte, _delimiter);
}

// testing only
void Scissors::sendText(String someText)
{
	theStream->println(someText);
}

// called by constructors 
void Scissors::init(char _start_byte, char _end_byte, char _delimiter)
{

	START_BYTE = _start_byte;
	END_BYTE = _end_byte;
	DELIMITER = _delimiter;
	MAX_ELEMENTS = 8;

	elementCount = 0;
	messageStart = -1;
	messageEnd = -1;
	delims[MAX_ELEMENTS + 1]; //   [MAX_ELEMENTS+1]   array to hold delimiter locations in buffer string
}


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// UI - listen for messages
// return INT == number of tokens in message
// this function needs to automatically handle available task, then error check, then get delims
// may be slow! 


// // need these ?
// int Scissors::available(){;}
// int Scissors::read(){;}


void Scissors::poll(){
	messageArrived = 1;  // set this to true -- we have a string
	messageOk = -1;
	messageParsed  = -1;
	elementCount = 0;

	// listen then call event 
    listen(); 

	if (elementCount > 0){
         scissorsEvent(messageBuffer);
	}
}

// UI - manual check for message and see if it is complete
// return INT == number of tokens, 0 = no tokens
// can get message length in global ?
// does NOT handle the message in any way

int Scissors::listen()
{
	if ( DEBUG >=2 ) Serial.println(F("listen"));

	// set message status -- globals may be over kill remove if we can
	// set message states to zero ...
	messageArrived = -1;
	messageOk = -1;
	messageParsed  = -1;
	elementCount = 0;

	// UI -- read the stream and get token count
	readStream(); // sets messageArrived to 1 or 0 for message or not.

	if (messageArrived == 1) // global -- have a raw buffer
	{
		getTokens(); // number of tokens is in elementCount
					 // messageParsed == 1 if this is successful
	}
	else // error of some kind early in process
	{
		if (DEBUG >=2) // at level 1 will fire every loop!
		{
			Serial.println(F("Read error - check stream, no message?"));
		}
	}

	return elementCount; // -1 or number of tokens
}

// for parsing with a provided string 
// returns token count
int Scissors::parse(String inString)
{
	if (DEBUG >=2) Serial.println(F("parse"));

	messageArrived = 1;  // set this to true -- we have a string
	messageOk = -1;
	messageParsed  = -1;
	elementCount = 0;

	messageBuffer = inString; // overwrite the messageBuffer - best way?
    getTokens();  // error check and delims 
	
	return elementCount; // 0 = none, >=1 ==> message
}

// need to change name of this to available
// check buffer for new data  - can be MQTT?  or does it have to be serial?
// mqtt is not a stream ... ???
int Scissors::readStream()
{
	if (DEBUG >=2) Serial.println(F("readStream"));

	messageLength = 0;
	messageArrived = 0; 
	if (useStream)      // changed flag to stream aug 2023
	{
		if (theStream->available())
		{
			messageBuffer = ""; // clear the buffer
			messageStart = -1;
			messageEnd = -1;
			// look into this -->
			delay(10); // for reasons I don't understand this is really important -- do not remove

			// J22 -- this must be blocking
			// go get all the characters in the buffer
			char c;
			while (theStream->available() > 0)
			{
				c = theStream->read(); // read char
				messageBuffer += c;			// store it in the string
				// Serial.println(F(c);
				messageLength++;
				delay(1); // ? really
			}
			
		} // END if stream available
		else {
			if (DEBUG >=2) Serial.println(F("readStream --> no new message"));
		}

		theStream->flush();

		if (messageLength > 0){
			messageArrived = 1;
		}
	}	// end useStream
	else {
			Serial.println(F("stream not selected"));
		}
	return messageArrived; // was state, which was from errorcheck
}


// this is called by .listen() and .parse()
// this checks for ok structure 
// THEN
// gets token count (stores delim locations)
// element count may be wrong -- may be one too low. 

int Scissors::getTokens(){

	if (DEBUG >=2) Serial.println(F("getTokens"));
	// confirm we have a message, then check and split it
    if (messageArrived == 1) {

		errorCheck(); // see if we have a structured message

			if (messageOk == 1) // message has correct structure
			{
				messageParsed = findDelimiters(); // returns 0,1  token # is in elementCount
			}
			else  // problem in errorCheck()
			{
				if (DEBUG)
				{
					Serial.println(F("getTokens --> message structure error, error check fail"));
				}
			}
	    return elementCount; 
	} else {
		return 0;
	}
}


// checks message for start, end and delimiter order (not count)
// sets and returns messageOk ==>returns 0 (error) or 1 (ok) 

int Scissors::errorCheck()
{
	if (DEBUG >=2) Serial.println(F("errorCheck"));

	messageOk = 0;

	int state = -1; // number of delimiters found
	int status = 7; // parse status, 1 = success

	messageStart = -1;
	messageEnd = -1;

	// a bit of error checking - ensure structure ok 
	int s = messageBuffer.indexOf(START_BYTE);
	int e = messageBuffer.indexOf(END_BYTE);
	int d = messageBuffer.lastIndexOf(DELIMITER);

	if (s == -1)
	{
		status = 6; // no start
		if (DEBUG)
			Serial.println(F("no start"));
	}
	else if (e == -1)
	{
		status = 5; // no end
		if (DEBUG)
			Serial.println(F("no end"));
	}
	else if (e < s)
	{
		status = 4;
		if (DEBUG)
			Serial.println(F("end before start"));
	}
	else if (s < e)
	{
		// have start and end - in correct order
		status = 1; // potential message
		if (DEBUG)
			Serial.println(F("start and end ok"));
	}

   // split this to make is a bit easier to read
   // now assuming we get here we have a start and end in correct order
	if ( status == 1 ) { // ok above
		// check for empty, s and e side by side
		if (s == e - 1)
		{
			status = 3;
			if (DEBUG)
				Serial.println(F("empty message"));
		} //  s==e-1
	}
	// message not empty ... 
	if ( status == 1 ) { // ok above
		// check if no DELIMITERS - may only have end byte - may have undelimited token
		if (d <= 0)
		{
				status = 2;
				if (DEBUG)
					Serial.println(F("no delims, but potential token"));
		
				// not sure what this is doing
				// set end of message to end byte
				e = messageBuffer.indexOf(END_BYTE);
		}
	}
    
	if ( status == 1 ) { // ok above
		// check for delim beside end
		//  if true make end = delim, not end byte
		if (d == e - 1)
		{
			e = d;
			if (DEBUG)
				Serial.println(F("remove comma before end"));
		}
	}

	// ok to parse - we have a structured message
	if (status == 1 || status == 2)
		{
			messageStart = s;
			messageEnd = e;
			messageOk = 1;
		}
		delay(1);
	
	// if (DEBUG >=2)
	// {
	// 	Serial.print(F("error check status "));
	// 	Serial.println(status);
	// }

	return messageOk; // was state == number of delims
}

int Scissors::findDelimiters()
{
	if (DEBUG >=2) Serial.println(F("findDelimiters"));

	// delims array stores locations of DELIMITERS
	messageParsed = 0; 
	elementCount = 0; // was 0 
	delims[elementCount] = messageStart; // was elementCount

	for (int i = messageStart + 1; i <= messageEnd; i++)
	{
	
		if ((messageBuffer.charAt(i) == DELIMITER) || (messageBuffer.charAt(i) == END_BYTE))
		{
			elementCount++;
			if (elementCount > MAX_ELEMENTS) //maxElements b/c we must store end of final token for parse
			{
				// this overflow is not YET signalled
				if (DEBUG)
				{
					Serial.print(F("message too long, message may be truncated"));
					// Serial.print("elementCount ");
					// Serial.println(elementCount);
				}

				break; // message is longer than max -- overflow
			} else {
				// Serial.print("new delim, count = ");
				// Serial.println(elementCount);
				
				delims[elementCount] = i;
				
				if (messageBuffer.charAt(i + 1) == END_BYTE)
				{
					for(int j=elementCount+1; j<MAX_ELEMENTS;j++){
						delims[j]=messageEnd;
					}
					break;
				}
			}

		} // end if

	} // end for

	// we are at end of message -- if elementCount still == 0 then there were no delimiters
	// however, we can only be here if we had an end byte after a start byte --
	// SO, there must be a single undelimited ELEMENT in the message
	if (elementCount == 0)
	{
		//	 if (DEBUG==1) Serial.println(F("no delims"));
		elementCount = 1;
		delims[1] = messageEnd;
	}

    // massive testing block -- comment to save memory
	// leave here in case you need it again 
	/* if (DEBUG)
	{   
		Serial.println(F("got ==> "));
		Serial.println(messageBuffer);
		for (int y=0;y<messageLength;y++){
			if ( y< 10){
			    Serial.print(y);
			}else {
				int f = y/10;
				Serial.print(y-f*10);
			}
		}
		Serial.println();
		Serial.print(F("s ==> "));
		Serial.println(messageStart);
		Serial.print(F("e ==> "));
		Serial.println(messageEnd);
		Serial.println(F("delims"));
		Serial.print(F("elementCount "));
		Serial.println(elementCount);
		for (int i=0; i < 8;i++)
		{
			Serial.print(delims[i]);
			Serial.print(' ');
		}
		Serial.println();
	} */

	if (elementCount > 0) messageParsed = 1;

	return elementCount;

} // end find delims

String Scissors::getElement(int whichOne)
{  
	// not called directly by user
    if (DEBUG >=2) Serial.println(F("getElement"));
    int flag = 0;
	sub = "";

	// whichone is an index, elementCount is a count
	// so second term of following compare has to be count-1
	if ((whichOne >= 0) && (whichOne <= (elementCount))) // <=
	{

		int startLoc = delims[whichOne] + 1; // start one char rt of INDEX
		int endLoc = delims[whichOne + 1];	 // end at next delim

		if (startLoc > endLoc) // START_BYTE comes after END_BYTE
		{
			sub = "GET ELEMENT - token error:: Start b4 End\n"; // start before end
		}
		else
		{ // START_BYTE preceeds END_BYTE

			for (int c = startLoc; c < endLoc; c++)
			{
				sub += messageBuffer.charAt(c);
			}
		}
	}
	else
	{
		sub = "GET ELEMENT ";
		sub += whichOne;
		sub += " token error: Element out of range\n"; // element out of range
	}
	return sub;
} // end getElement

int Scissors::getInt(int whichOne)
{

	// http://www.cplusplus.com/reference/clibrary/cstdlib/atoi/
	// no standard way to solve issue of STRING input not being in range of INT
	// this will fail QUITELY!!!!

	int lowIndex = 0;

	String elementString = getElement(whichOne);

	char char_buff[elementString.length() + 1];

	elementString.toCharArray(char_buff, sizeof(char_buff));

	// check for neg -- without this we reject neg values
	if (char_buff[0] == '-')
	{
		lowIndex = 1;
		// if (DEBUG==1) Serial.println(F("neg val"));
	}

	if ((char_buff[lowIndex] < '0' || char_buff[0] > '9'))
	{
		if (DEBUG)
		{
			Serial.print(F("NaN :: "));
		    Serial.println(char_buff);
		}
		return -9999;
	}

	return atoi(char_buff);
}

float Scissors::getFloat(int whichOne)
{

	int lowIndex = 0;

	String elementString = getElement(whichOne);

	char char_buff[elementString.length() + 1];

	elementString.toCharArray(char_buff, sizeof(char_buff));

	// check for neg -- without this we reject neg values
	if (char_buff[0] == '-')
	{
		lowIndex = 1;
		delay(10);
	}

	if (char_buff[lowIndex] < '0' || char_buff[0] > '9')
	{
		if (DEBUG)
			Serial.println(F("NaN"));
		return -9999;
	}

	return atof(char_buff);
}

String Scissors::getString(int whichOne)
{
	return getElement(whichOne);
}

String Scissors::getRaw()
{
	return messageBuffer;
}

int Scissors::setStartByte(char s)
{
	START_BYTE = s;
	return 1;
}

int Scissors::setEndByte(char e)
{
	END_BYTE = e;
	return 1;
}

int Scissors::setDelimiter(char d)
{
	DELIMITER = d;
	return 1;
}

int Scissors::setMaxElements(int m)
{
	MAX_ELEMENTS = m;
	delims[MAX_ELEMENTS + 1];
	return 1;
}

char Scissors::getStartByte()
{
	return START_BYTE;
}

char Scissors::getEndByte()
{
	return END_BYTE;
}

char Scissors::getDelimiter()
{
	return DELIMITER;
}

int Scissors::getMaxElements()
{
	return MAX_ELEMENTS;
}

int Scissors::getBaud()
{
	return BAUD;
}

int Scissors::setDebug(int debugLevel)
{
	DEBUG = debugLevel; // 0 none, 1 verbose, 2= trace (main function names as well)
	return 1;
}


int Scissors::getMessageLength()
{
	return messageLength;
}


int Scissors::getTokenCount()
{
	return elementCount; // may need to add 1 ? 
}
