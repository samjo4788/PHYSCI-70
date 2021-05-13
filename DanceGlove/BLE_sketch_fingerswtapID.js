// orientation variables, right
let tapID = 0.0;
let valflex1 = 0.0;
let valflex2 = 0.0;


/* initialize services we're looking for*/
//right
const serviceUuid = "19b10010-e8f2-537e-4f6c-d104768a1214";
const characteristicsUUID = {
  tapID: "19b10013-e8f2-537e-4f6c-d104768a1214",
  valflex1: "19b10014-e8f2-537e-4f6c-d104768a1214",
  valflex2: "19b10015-e8f2-537e-4f6c-d104768a1214",
}

// Declare BLE Object + characteristics
let myBLE;
let tapIDCharacteristic;
let valflex1Characteristic;
let valflex2Characteristic;


function setup() {
  createCanvas(720, 500);
  myBLE = new p5ble();
  background("#FFF");
  // make a button and give it a callback
  const connectButton = createButton('Connect and Start Notifications')
  connectButton.mousePressed(connectAndStartNotify);


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
    else if (characteristics[i].uuid == characteristicsUUID.tapID) {
      tapIDCharacteristic = characteristics[i];
      myBLE.startNotifications(tapIDCharacteristic, tapIDCallback);
    } 
    else if (characteristics[i].uuid == characteristicsUUID.valflex1){
      valflex1Characteristic = characteristics[i];
      myBLE.startNotifications(valflex1Characteristic, valflex1Callback);
    } 
    else if (characteristics[i].uuid == characteristicsUUID.valflex2) {
      valflex2Characteristic = characteristics[i];
      myBLE.startNotifications(valflex2Characteristic, valflex2Callback);
    } 
    else {
      console.log("nothing");
    }
  }
}


function tapIDCallback(data) {
  console.log('tapID: ', data)
  console.log(Number(data))
  tapID = data;
}

function valflex1Callback(data) {
  console.log('valflex1: ', data)
  valflex1 = data;
}

function valflex2Callback(data) {
  console.log('valflex2: ', data)
  valflex2 = data;
}



//draws triangles
//index finger triangle
// function drawIndex() {
//   stroke(0)
//   fill(180)
//   triangle(180,50,270,((valflex1/800)*400),360,50)

// //draw middle finger triangle
//   stroke(0)
// //   fill(180)
//   triangle(360,50,450,((valflex2/800)*400),540,50)
// }
// draws the Arduino Nano:
// function drawMiddle() {
//   // the base board:
// 	stroke(0) // outline black
// 	fill(180) // fill is grey
// 	box(60,0,60)
// }

// function drawIndex() {
// 	stroke(0) // outline black
// 	box(60,0,60)//  fill(180) // fill is grey

// }


function draw() {
  // update the drawing:
  background(255); // set background to white
  //index finger triangle

//push();
  
if(tapID === 1) {
  stroke(0);
  fill(0);
  triangle(180,300,270,(400*(valflex1/800)),360,300);

  //draw middle finger triangle
  stroke(0);
  fill(180);
  triangle(360,300,450,(400*(valflex2/800)),540,300);

}
else if (tapID === 2) {
  stroke(0);
  fill(180);
  triangle(180,300,270,(400*(valflex1/800)),360,300);

  //draw middle finger triangle
  stroke(0);
  fill(0);
  triangle(360,300,450,(400*(valflex2/800)),540,300);
}

else if(tapID === 3) {

  stroke(0);
  fill(0);
  triangle(180,300,270,(400*(valflex1/800)),360,300);

  //draw middle finger triangle
  stroke(0);
  fill(0);
  triangle(360,300,450,(400*(valflex2/800)),540,300);
}

else {

  stroke(0);
  fill(180);
  triangle(180,300,270,(400*(valflex1/800)),360,300);
//draw middle finger triangle
  stroke(0);
  fill(180);
  triangle(360,300,450,(400*(valflex2/800)),540,300);

//pop();
}

}