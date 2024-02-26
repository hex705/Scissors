// simple serial
// https://github.com/p5-serial/p5.serialport
// needs serial control app

let serial;
let portName = "/dev/tty.usbmodem142201";
let theCanvas;
let cycleLength=50;
let duty = 0;
let oldCycle;

function setup() {
  theCanvas = createCanvas(550, 100);

  // barebones serial implimentation -- talking only
  serial = new p5.SerialPort();    // make a new instance of the
  serial.open(portName);           // open a serial port
}

function draw() {
 drawBackground();
 let selectedDutyCycle = handleMouse(); // draws mouse location in red
                                        // and returns the selected cycle (0-10)

 //map the selection to ON / OFF time for LED (cycleDuration is 1000mS)
 let onTime = selectedDutyCycle*100; // scale mousePosition to time (mS) 0 -1000
 let offTime = 1000 - onTime;        // basic cycleLength is 1000 mS

 // send values to arduino
 let message = ("*"  + onTime+ "," + offTime + ",#"); // onTime, offTime

 // only send the cycle once -- otherwise it fills the arduino buffer
 if ( selectedDutyCycle != oldCycle) {
   serial.write(message);
   console.log(message); // uncomment if you want to see it in browser console
 }
   oldCycle = selectedDutyCycle;
}

function drawBackground(){
 // set colors
 background (255);
 noFill();
 stroke(0);
 strokeWeight(2);
 rect(0,0,width,height);
 fill( 0 );
 noStroke();

 // draw duty cycles
 duty = 0;
 for (let i = 0; i < 11; i ++ ) {  // 5 white rectangles
     rect ( ((i*cycleLength)+duty), 0, cycleLength - duty , height);
     duty +=5;
 }
} // end drawBackground


function handleMouse() {
  // set colors
  stroke(255,0,0);
  fill(255,0,0,20);
  strokeWeight(4);

  // determine which cycle we are hovering over
  let selectedCycle = parseInt(mouseX / cycleLength);
  rect (selectedCycle*cycleLength,0,cycleLength,height);
  return selectedCycle;
}
