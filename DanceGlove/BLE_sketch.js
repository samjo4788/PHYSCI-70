// orientation variables:
let heading = 0.0;
let pitch = 0.0;
let roll = 0.0;

/* initialize services we're looking for*/
const serviceUuid = "19b10010-e8f2-537e-4f6c-d104768a1214";
const characteristicsUUID = {
  led: "19b10011-e8f2-537e-4f6c-d104768a1214",
  button: "19b10012-e8f2-537e-4f6c-d104768a1214",
  heading: "19b10013-e8f2-537e-4f6c-d104768a1214",
  pitch: "19b10014-e8f2-537e-4f6c-d104768a1214",
  roll: "19b10015-e8f2-537e-4f6c-d104768a1214"
}

// Declare BLE Object + characteristics
let myBLE;
let buttonCharacteristic;
let ledCharacteristic;
let headingCharacteristic;
let pitchCharacteristic;
let rollCharacteristic;


let buttonValue = 0;

function setup() {
  createCanvas(500, 600, WEBGL);
  myBLE = new p5ble();
  background("#FFF");
  // make a button and give it a callback
  const connectButton = createButton('Connect and Start Notifications')
  connectButton.mousePressed(connectAndStartNotify);
  const toggleButton = createButton('Toggle');

  toggleButton.position(15, 15);
  toggleButton.mousePressed(toggleLED);
}

function connectAndStartNotify() {
  myBLE.connect(serviceUuid, gotCharacteristics);
  //myBLE.connect(gattServiceUuid, gattCharacteristics);
}

function gotCharacteristics(error, characteristics) {
  if (error) console.log('error: ', error);
  console.log(characteristics[1].uuid);
  for (let i = 0; i < characteristics.length; i++) {
    if (characteristics[i].uuid == characteristicsUUID.button) {
      buttonCharacteristic = characteristics[i];
      myBLE.startNotifications(buttonCharacteristic, handleButton);
    } 
    else if (characteristics[i].uuid == characteristicsUUID.led) {
      ledCharacteristic = characteristics[i];
    } 
    else if (characteristics[i].uuid == characteristicsUUID.heading) 
    {
      headingCharacteristic = characteristics[i];
      myBLE.startNotifications(headingCharacteristic, headingCallback,'float32');
    } 
    else if (characteristics[i].uuid == characteristicsUUID.pitch) {
      pitchCharacteristic = characteristics[i];
      myBLE.startNotifications(pitchCharacteristic, pitchCallback, 'float32');
    } 
    else if (characteristics[i].uuid == characteristicsUUID.roll) {
      rollCharacteristic = characteristics[i];
      myBLE.startNotifications(rollCharacteristic, rollCallback, 'float32');
    } 
    else {
      console.log("nothing");
    }
  }
}

function headingCallback(data) {
  console.log('heading: ', data)
  console.log(Number(data))
  heading = data;
}

function pitchCallback(data) {
  console.log('pitch: ', data)
  pitch = data;
}

function rollCallback(data) {
  console.log('roll: ', data)
  roll = data;
}

function handleButton(data) {
  console.log('Button: ', data);
  buttonValue = Number(data);
}

function toggleLED() {
  myBLE.read(ledCharacteristic, handleLED);
}

function handleLED(error, data) {
  if (error) console.log('error: ', error);
  console.log('LED: ', data);
  myBLE.write(ledCharacteristic, !data);
}


// draws the Arduino Nano:
function drawArduino() {
  // the base board:
  stroke(0, 90, 90); // set outline color to darker teal
  fill(0, 130, 130); // set fill color to lighter teal
  box(300, 10, 120); // draw Arduino board base shape

  // the CPU:
  stroke(0); // set outline color to black
  fill(80); // set fill color to dark grey
  translate(30, -6, 0); // move to correct position
  box(60, 0, 60); // draw box

  // the radio module:
  stroke(80); // set outline color to grey
  fill(180); // set fill color to light grey
  translate(80, 0, 0); // move to correct position
  box(60, 15, 60); // draw box

  // the USB connector:
  translate(-245, 0, 0); // move to correct position
  box(35, 15, 40); // draw box
}

function draw() {
  // update the drawing:
  background(255); // set background to white
  push(); // begin object to draw
  // variables for matrix translation:
  let c1 = cos(radians(roll));
  let s1 = sin(radians(roll));
  let c2 = cos(radians(pitch));
  let s2 = sin(radians(pitch));
  let c3 = cos(radians(heading));
  let s3 = sin(radians(heading));
  applyMatrix(c2 * c3, s1 * s3 + c1 * c3 * s2,
    c3 * s1 * s2 - c1 * s3, 0, -s2, c1 * c2,
    c2 * s1, 0, c2 * s3, c1 * s2 * s3 - c3 * s1,
    c1 * c3 + s1 * s2 * s3, 0, 0, 0, 0, 1);

  // draw arduino board:
  drawArduino();
  pop(); // end of object
}