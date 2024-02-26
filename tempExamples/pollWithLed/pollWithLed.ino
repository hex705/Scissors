// Scissors Example:  pollWithLed

// pollWithLed uses Scissors library .poll() function to trigger scissorsEvent().
// Strings from serial (Arduino Stream) are used.

// this example demonstrates passing a string into two values
// the values control on and off time of a blinking LED. 

// setting the LED_STATE is handled with a pair of NON-BLOCKING timers.

// default message structure
// start     = '*'
// end       = '#'
// delimeter = ','
// message structure :: *data0,data1#

// can parse up to 8 data tokens -- may get slower at that level (YMMV)

// Scissors and this Example by hex705 (Steve Daniels)

// January 2012, All Rights Reserved.
// updated Jan 2022, Jan 2024

// Scissors and similar Libraries can be found at:
// https://github.com/hex705


#include <Scissors.h>
Scissors cut;

// led controls
int ledPin = 13;
int on = 1000; // defaults
int off = 500;

// timer and debug variables
int LED_STATE = 0;
int LAST_STATE = 0;

unsigned long timeNow;
unsigned long startTime;


void setup() {
  Serial.begin(9600); // start serial but do not connect it to scissors
  cut.begin(Serial);  // no stream attached

  pinMode(ledPin,OUTPUT);

  // instructions
  Serial.println(__FILE__);
  Serial.println();

  Serial.println("Default pattern: ON - 1000mS, OFF - 500mS");
  Serial.println("");
  Serial.println("Send message of form: *OnTime,OffTime#");
  Serial.println("\nUse this structure in Serial monitor, eg:");
  Serial.println("*250,250,#");
}

void loop() {

    blink(on,off); // uses timers
  
    cut.poll();  // data from Serial stream. 
}

void scissorsEvent( String &theMessage ){  // its just a string

  // data extraction from incoming message
  // on/off are global for use outside event. 
    Serial.println("\nGetting data.");
    on  = cut.getInt(0);
    off = cut.getInt(1);

  // show full message
    Serial.print("Received: ");
    Serial.println(theMessage);

  // output the new pattern
    Serial.println("New pattern: ");
    Serial.print("on = ");
    Serial.println(on);
    Serial.print("off = ");
    Serial.println(off);
    Serial.println();
  
}

void blink (int onTime, int offTime) {

  // uses timers so that we can update on/off at any point in the cycle
  // try commenting this block out and using delay example (below) to see why.

  timeNow = millis();

  // error check your data -- always
  // this is a timer so it needs positive numbers
  if ( onTime < 0 ) {
    onTime = 0;
    LED_STATE = 0;
    Serial.println("onTime set to zero");
  }

  if ( offTime < 0 ) {
    offTime = 0;
    Serial.println("offTime set to zero");
  }

  // timers -- one for each LED state
  if ( LED_STATE == HIGH) {
    if ( timeNow - startTime > onTime){
      LED_STATE = LOW;
      startTime += onTime;
    }
  }

  if ( LED_STATE == LOW) {
    if ( timeNow - startTime > offTime){
      LED_STATE = HIGH;
      startTime += offTime;
    }
  }

// ***************  DELAY SOLUTION
// BLOCKING!  If we set the blink phases to long durations
// we cannot update the inputs in real time.

// Uncomment if you want to see it in action.
//
//       digitalWrite(ledPin,HIGH);
//       delay(onTime);
//
//       digitalWrite(ledPin,LOW);
//       delay(offTime);


  // prints current settings on each change
 // if ( LAST_STATE!=LED_STATE) {
  // if ( lastOn != on || lastOff != off) {
  //     Serial.println("new pattern arrived: ");
  //     Serial.print("on = ");
  //     Serial.println(onTime);
  //     Serial.print("off = ");
  //     Serial.println(offTime);
  //         Serial.print("last on = ");
  //     Serial.println(lastOn );
  //     Serial.print(" last off = ");
  //     Serial.println(lastOff);
  //     Serial.println();
  // }

  digitalWrite(ledPin,LED_STATE);

}
