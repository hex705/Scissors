
// Scissors v0.02
// Scissors Example:  LED_blink_noSTREAM

// LED_blink_noSTREAM uses Scissors to parse strings originating
// outside serial streams (eg MQTT libraries)

// this example demonstrated passing a string into 
// the parse function directly

// default message structure
// start     = '*'
// end       = '#'
// delimeter = ','
// message strcuture :: *data0,data1#

// can parse up to 8 data tokens -- may get slower at that level (YMMV)

// Scissors and this Example by hex705 (Steve Daniels)

// January 2012, All Rights Reserved.
// updated Jan 2022

// Scissors and similar Libraries can be found at:
// https://github.com/hex705


#include <Scissors.h>
Scissors cut;

// led controls
int ledPin = 13;
int on = 0;
int off = 0;

// timer and debug variables
int LED_STATE = 1;
int LAST_STATE = 0;
unsigned long timeNow;
unsigned long startTime;

// string to parse - can come from monitor following this format.
String s = ("*1000,500#"); // on time , off time -- feel free to change it


void setup() {
  Serial.begin(9600); // start serial but do not connect it to scissors
  cut.begin();
  pinMode(ledPin,OUTPUT);
}

void loop() {

    cut.parse(s);
    on = cut.getInt(0);
    off= cut.getInt(1);

    blink(on,off);
}

// uses a timer so that we can update the times during each phase
// try with delay (below) to see why.

void blink (int onTime, int offTime) {

  timeNow = millis();

  // error check your data -- always
  // this is a timer so need positive numbers
  if ( onTime < 0 ) {
    onTime = 0;
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
  if ( LAST_STATE!=LED_STATE) {
      Serial.println("changed state");
      Serial.print("on = ");
      Serial.println(onTime);
      Serial.print("off = ");
      Serial.println(offTime);
      Serial.println();
  }

  digitalWrite(ledPin,LED_STATE);
  LAST_STATE = LED_STATE;
}
