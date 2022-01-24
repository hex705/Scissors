
// Scissors v0.01
// Scissors Example:  mouseBlink (v1)

// mouseBlink uses Scissors to parse messages from a Processing sketch (below)
// and uses data to control blink rate of LED on pin13

// Scissors and this Example by hex705 (Steve Daniels)
// August 2012, All Rights Reserved.

// Scissors and similar Libraries can be found at:
// https://github.com/hex705


#include <Scissors.h>

Scissors cut;
int ledPin = 5;

int on = 0;
int off = 0;

void setup() {

  cut.begin(19200);
  pinMode(ledPin,OUTPUT);

}


void loop() {

  if (cut.update() > 0) { // have a new message?

        // get the values
        on = cut.getInt(0);
        off= cut.getInt(1);

        Serial.println("\nrecieved:");
        Serial.print("on = ");
        Serial.println(on);
        Serial.print("off = ");
        Serial.println(off);
        Serial.println();
  }
    blink(on,off);
}


void blink (int onTime, int offTime) {

       digitalWrite(ledPin,HIGH);
       delay(onTime);

       digitalWrite(ledPin,LOW);
       delay(offTime);

}




//  ****** Companion Processing Sketch ******



// Copy and paste the following sketch into Processing
// Select ALL and uncomment ( Processing --> Edit --> Comment/unComment)
// RUN the Processing sketch *AFTER* you have uploaded mouseBlink to your Arduino.




//// Scissors v0.01
//// Scissors Example:  Prcessing Sketch for mouseBlink (v1)
//
//// mouseBlink uses Scissors to parse messages from this Processing sketch
//// and uses data to control blink rate of LED on pin13
//
//// Scissors and this Example by hex705 (Steve Daniels)
//// August 2012, All Rights Reserved.
//
//// Scissors and similar Libraries can be followed at:
//// https://github.com/hex705
//
//
//
//
//  // import Libraries
//  import processing.serial.*;
//
//  // variables
//  Serial myPort;
//  int cycleLength = 50;
//  int duty = 0;
//
//
//void setup() {
//  frameRate(10);
//
//  size(550,100);
//
//  // List all the available serial ports
//  println(Serial.list());
//
//  // connect this sketch to the USB port or your Arduino at specified BAUD
//  myPort = new Serial(this, Serial.list()[0], 19200);
//
//}
//
//
//void draw() {
//
//  drawBackground();
//
//  int selectedDutyCycle = handleMouse(); // draws mouse location in red
//                                         // and returns the selected cycle (0-10)
//
//  //map the selection to ON / OFF time for LED (cycleDuration is 1000mS)
//
//  int onTime = selectedDutyCycle*100; // scale mousePosition to time (mS) 0 -1000
//  int offTime = 1000 - onTime;        // basic cycleLength is 1000 mS
//
//
//
//  // send values to arduino
//  String message = ("*"  + onTime+ "," + offTime + ",#"); // onTime, offTime
//  myPort.write(message);
//  println(message);
//
//}
//
//
//
//void drawBackground(){
//
//  // set colors
//  background (255);
//  fill( 0 );
//  noStroke();
//
//  // draw duty cycles
//  duty = 0;
//  for (int i = 0; i < 11; i ++ ) {  // 5 white rectangles
//      rect ( ((i*cycleLength)+duty), 0, cycleLength - duty , height);
//      duty +=5;
//  }
//
//} // end drawBackground
//
//
//
//int handleMouse() {
//
//   // set colors
//   stroke(255,0,0);
//   fill(255,0,0,20);
//   strokeWeight(4);
//
//   // determine which cycle we are hovering over
//   int selectedCycle = mouseX / cycleLength;
//
//   rect (selectedCycle*cycleLength,0,cycleLength,height);
//
//   return selectedCycle;
//}
//
//
