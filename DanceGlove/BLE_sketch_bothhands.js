// orientation variables, right
let heading = 0.0;
let pitch = 0.0;
let roll = 0.0;

//proximity variable, right
let proximity = 0.0;

// orientation variables, left
let Lheading = 0.0;
let Lpitch = 0.0;
let Lroll = 0.0;

//proximity variable, left
let Lproximity = 0.0;

/* initialize services we're looking for*/
//right
const serviceUuid = "19b10010-e8f2-537e-4f6c-d104768a1214";
const characteristicsUUID = {
  heading: "19b10013-e8f2-537e-4f6c-d104768a1214",
  pitch: "19b10014-e8f2-537e-4f6c-d104768a1214",
  roll: "19b10015-e8f2-537e-4f6c-d104768a1214",
  proximity: "87d96aa2-50ff-45a7-bcae-84dbb7b30a5b", 
}

//left
const LserviceUuid = "6b269528-fc66-4f70-866e-0d567920b533";
const LcharacteristicsUUID = {
  Lheading: "ae16b0a2-eae7-4156-9b72-3a9a482bf66c",
  Lpitch: "8edec104-0427-4dca-b3aa-891ebb5de90e",
  Lroll: "2b90f810-232a-496b-ac79-c6aa797173a3",
  Lproximity: "6d211bf8-df1f-4d92-8955-a87c8e03d1e6", 
}
// Declare BLE Object + characteristics
let myBLE;
let headingCharacteristic;
let pitchCharacteristic;
let rollCharacteristic;
let proximityCharacteristic;
let LheadingCharacteristic;
let LpitchCharacteristic;
let LrollCharacteristic;
let LproximityCharacteristic;


function setup() {
  createCanvas(720, 400, WEBGL);
  myBLE = new p5ble();
  background("#FFF");
  // make a button and give it a callback
  const connectButton = createButton('Connect and Start Notifications')
  connectButton.mousePressed(connectAndStartNotify);
}

function connectAndStartNotify() {
  myBLE.connect(serviceUuid, gotRCharacteristics);
  myBLE.connect(LserviceUuid, gotLCharacteristics);
  //myBLE.connect(gattServiceUuid, gattCharacteristics);
}

function gotRCharacteristics(error, characteristics) {
  if (error) console.log('error: ', error);
  console.log(characteristics[1].uuid);
  for (let i = 0; i < characteristics.length; i++) {
    if (characteristics[i].uuid == characteristicsUUID.button) {
      buttonCharacteristic = characteristics[i];
      myBLE.startNotifications(buttonCharacteristic, handleButton);
    } 
    else if (characteristics[i].uuid == characteristicsUUID.proximity) {
      proximityCharacteristic = characteristics[i];
      myBLE.startNotifications(proximityCharacteristic, proximityCallback,'float32');
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

function gotLCharacteristics(error, characteristics) {
  if (error) console.log('error: ', error);
  console.log(characteristics[1].uuid);
  for (let i = 0; i < characteristics.length; i++) {
    if (characteristics[i].uuid == characteristicsUUID.Lproximity) {
      LproximityCharacteristic = characteristics[i];
      myBLE.startNotifications(LproximityCharacteristic, LproximityCallback,'float32');
    } 
    else if (characteristics[i].uuid == characteristicsUUID.Lheading) 
    {
      LheadingCharacteristic = characteristics[i];
      myBLE.startNotifications(LheadingCharacteristic, LheadingCallback,'float32');
    } 
    else if (characteristics[i].uuid == characteristicsUUID.Lpitch) {
      LpitchCharacteristic = characteristics[i];
      myBLE.startNotifications(LpitchCharacteristic, LpitchCallback, 'float32');
    } 
    else if (characteristics[i].uuid == characteristicsUUID.Lroll) {
      LrollCharacteristic = characteristics[i];
      myBLE.startNotifications(LrollCharacteristic, LrollCallback, 'float32');
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

function proximityCallback(data) {
  console.log('proximity: ', data);
  proximity = data;
}

function LheadingCallback(data) {
  console.log('Lheading: ', data)
  console.log(Number(data))
  Lheading = data;
}

function LpitchCallback(data) {
  console.log('Lpitch: ', data)
  Lpitch = data;
}

function LrollCallback(data) {
  console.log('Lroll: ', data)
  Lroll = data;
}

function LproximityCallback(data) {
  console.log('Lproximity: ', data);
  Lproximity = data;
}




// draws the Arduino Nano:
function drawRight() {
  // the base board:
  translate(240,0)
  stroke(0, 90, 90); // set outline color to darker teal
  fill(0, 130, 130); // set fill color to lighter teal
  plane(120); // draw a torus shape

  // the CPU:
  //stroke(0); // set outline color to black
  //fill(80); // set fill color to dark grey
  //translate(30, -6, 0); // move to correct position
  //box(60, 0, 60); // draw box

  // the radio module:
  //stroke(80); // set outline color to grey
  //fill(180); // set fill color to light grey
  //translate(80, 0, 0); // move to correct position
  //box(60, 15, 60); // draw box

  // the USB connector:
  //translate(-245, 0, 0); // move to correct position
  //torus(35, 15, 40); // draw box
}

function drawLeft() {
  translate(240,0)
  stroke(0); // set outline color to black
  fill(180); // set fill color to light grey
  plane(120); // draw a plane
}

function draw() {
  // update the drawing:
  background(255); // set background to white
  
  push(); // begin object to draw -- right hand
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
  drawRight();
  pop(); // end of object

  push(); // begin object to draw -- left hand
  // variables for matrix translation:
  let c1L = cos(radians(Lroll));
  let s1L = sin(radians(Lroll));
  let c2L = cos(radians(Lpitch));
  let s2L = sin(radians(Lpitch));
  let c3L = cos(radians(Lheading));
  let s3L = sin(radians(Lheading));
  applyMatrix(c2L * c3L, s1L * s3L + c1L * c3L * s2L,
    c3L * s1L * s2L - c1L * s3L, 0, -s2L, c1L * c2L,
    c2L * s1L, 0, c2L * s3L, c1L * s2L * s3L - c3L * s1L,
    c1L * c3L + s1L * s2L * s3L, 0, 0, 0, 0, 1);

  // draw arduino board:
  drawLeft();
  pop(); // end of object
}