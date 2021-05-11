// orientation variables, right
let accelx = 0.0;
let accely = 0.0;
let accelz = 0.0;


/* initialize services we're looking for*/
//right
const serviceUuid = "19b10010-e8f2-537e-4f6c-d104768a1214";
const characteristicsUUID = {
  accelx: "19b10013-e8f2-537e-4f6c-d104768a1214",
  accely: "19b10014-e8f2-537e-4f6c-d104768a1214",
  accelz: "19b10015-e8f2-537e-4f6c-d104768a1214",
}

// Declare BLE Object + characteristics
let myBLE;
let accelxCharacteristic;
let accelyCharacteristic;
let accelzCharacteristic;


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
    else if (characteristics[i].uuid == characteristicsUUID.accelx) {
      accelxCharacteristic = characteristics[i];
      myBLE.startNotifications(accelxCharacteristic, accelxCallback,'float32');
    } 
    else if (characteristics[i].uuid == characteristicsUUID.accely){
      accelyCharacteristic = characteristics[i];
      myBLE.startNotifications(accelyCharacteristic, accelyCallback,'float32');
    } 
    else if (characteristics[i].uuid == characteristicsUUID.accelz) {
      accelzCharacteristic = characteristics[i];
      myBLE.startNotifications(accelzCharacteristic, accelzCallback, 'float32');
    } 
    else {
      console.log("nothing");
    }
  }
}


function accelxCallback(data) {
  console.log('heading: ', data)
  console.log(Number(data))
  accelx = data;
}

function accelyCallback(data) {
  console.log('pitch: ', data)
  accely = data;
}

function accelzCallback(data) {
  console.log('roll: ', data)
  accelz = data;
}





// draws the Arduino Nano:
function drawRight() {
  // the base board:
  stroke(0, 90, 90); // set outline color to darker teal
  fill(0, 130, 130); // set fill color to lighter teal
  plane(abs(accelz)); // draw a torus shape

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


function draw() {
  // update the drawing:
  background(255); // set background to white
  
  push(); // begin object to draw -- right hand
  
  translate(width/accelx, height/accely) // keep playing around with this

  // variables for matrix translation:
  // let c1 = cos(radians(roll));
  // let s1 = sin(radians(roll));
  // let c2 = cos(radians(pitch));
  // let s2 = sin(radians(pitch));
  // let c3 = cos(radians(heading));
  // let s3 = sin(radians(heading));
  // applyMatrix(c2 * c3, s1 * s3 + c1 * c3 * s2,
  //   c3 * s1 * s2 - c1 * s3, 0, -s2, c1 * c2,
  //   c2 * s1, 0, c2 * s3, c1 * s2 * s3 - c3 * s1,
  //   c1 * c3 + s1 * s2 * s3, 0, 0, 0, 0, 1);

  // draw arduino board:
  drawRight();
  pop(); // end of object
}