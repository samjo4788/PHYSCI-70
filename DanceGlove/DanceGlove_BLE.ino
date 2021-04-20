#include "Arduino_LSM9DS1.h"
#include "MadgwickAHRS.h"
#include "ArduinoBLE.h"

// initialize a Madgwick filter:
Madgwick filter;

// sensor's sample rate is fixed at 119 Hz:
const float sensorRate = 119.00;

// values for orientation:
float roll = 0.0;
float pitch = 0.0;
float heading = 0.0;

// variables for error detection and debugging
// 1 = issue setting up IMU
// 2 = issue setting up BLE


// BLE LED Service
BLEService ledService("19B10010-E8F2-537E-4F6C-D104768A1214"); // create service
const int ledPin = LED_BUILTIN; // set ledPin to on-board LED
const int buttonPin = 4; // set buttonPin to digital pin 4
// create switch characteristic and allow remote device to read and write
BLEByteCharacteristic ledCharacteristic("19B10011-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
// create button characteristic and allow remote device to get notifications
BLEByteCharacteristic buttonCharacteristic("19B10012-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify);


// BLE Generic Attribute Service (ONLY IF DOING THIS THE RIGHT WAY) 
//BLEService gattService("180A"); // create service

// a characteristic for each of the plane angles heading, pitch and roll
BLEFloatCharacteristic headingCharacteristic("19B10013-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify);
BLEFloatCharacteristic pitchCharacteristic("19B10014-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify);
BLEFloatCharacteristic rollCharacteristic("19B10015-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify);

// OK SO THE PROPER WAY TO DO THIS WOULD BE TO USE THE BLE CHARACTERISTIC PLANE ANGLE (0x2763) AND 
// THEN USE DESCRIPTORS TO DIFFERENTIATE ROLL/PITCH/HEADING FROM ONE ANOTHER, BUT P5 DOES NOT SUPPORT
// DESCRIPTORS, NOR DOES IT SEEM TO SUPPORT CONNECTING TO MULTIPLE SERVICES, SO WE CAN MAKE IT EASY AND
// CREATE ALL CUSTOM CHARACTERISTICS AND LUMP THEM INTO LEDSERVICE.


// Get Serial, BLE and IMU going
void setup() {
  Serial.begin(9600);
  
  pinMode(ledPin, OUTPUT); // use the LED as an output
  pinMode(buttonPin, INPUT); // use button pin as an input
  
  // Turn LED on
  digitalWrite(ledPin, HIGH); 
  
  // attempt to start the IMU:
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU");
    // stop here if you can't access the IMU:
    Serial.end(); 
    err_func(1); 
  }

  // start the Madgwick filter to run at the sample rate:
  filter.begin(sensorRate);

  // Start the BLE Radio
  if (!BLE.begin()) {
      Serial.println("starting BLE failed!");
      err_func(2); 
  }

  // set the local name peripheral advertises
  BLE.setLocalName("DanceGlove");
  // set the UUID for the service this peripheral advertises:
  BLE.setAdvertisedService(ledService);
//  BLE.setAdvertisedService(gattService);
  
  // add the characteristics to the service
  ledService.addCharacteristic(ledCharacteristic);
  ledService.addCharacteristic(buttonCharacteristic);
  ledService.addCharacteristic(headingCharacteristic);
  ledService.addCharacteristic(pitchCharacteristic);
  ledService.addCharacteristic(rollCharacteristic);

  // add the service
  BLE.addService(ledService);
  //BLE.addService(gattService);  

  // init all values to 0
  ledCharacteristic.writeValue(0);
  buttonCharacteristic.writeValue(0);
  headingCharacteristic.writeValue(0);
  pitchCharacteristic.writeValue(0);
  rollCharacteristic.writeValue(0);

  // start advertising
  BLE.advertise();  
  
  Serial.println("Bluetooth device active, waiting for connections...");
}

void err_func(int err_code){
  // stay here forever; TODO: Try to recover
  // LED will turn on for ~1s, the off for err_code seconds then repeat
  while (true) {
    digitalWrite(ledPin, HIGH); 
    delay(1000);
    digitalWrite(ledPin, LOW); 
    delay(1000*err_code);
  }
}
 
void loop() {
  // poll for BLE events
  BLE.poll();
    
  // values for acceleration and rotation:
  float xAcc, yAcc, zAcc;
  float xGyro, yGyro, zGyro;
  bool ping = false; // variable tells us if data was requested over serial;

  
  // check if the IMU is ready to read:
  if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable()) {
    // read accelerometer and gyrometer:
    IMU.readAcceleration(xAcc, yAcc, zAcc);
    IMU.readGyroscope(xGyro, yGyro, zGyro);
 
    // update the filter, which computes orientation:
    filter.updateIMU(xGyro, yGyro, zGyro, xAcc, yAcc, zAcc);
 
    // print the heading, pitch and roll
    roll = filter.getRoll();
    pitch = filter.getPitch();
    heading = filter.getYaw();
  }
 
  // if you get a byte in the serial port,
  // send the latest heading, pitch, and roll:
  while(Serial.available()) {
    Serial.read(); 
    ping = true; 
    }
  if (ping) {
    Serial.print(heading);
    Serial.print(",");
    Serial.print(pitch);
    Serial.print(",");
    Serial.println(roll);
  }

  // read the current button pin state
  char buttonValue = digitalRead(buttonPin);

  // has the value changed since the last read
  boolean buttonChanged = (buttonCharacteristic.value() != buttonValue);

  if (buttonChanged) {
    // button state changed, update characteristics
    ledCharacteristic.writeValue(buttonValue);
    buttonCharacteristic.writeValue(buttonValue);
  }

  if (ledCharacteristic.written() || buttonChanged) {
    // update LED, either central has written to characteristic or button state has changed
    if (ledCharacteristic.value()) {
      Serial.println("LED on");
      digitalWrite(ledPin, HIGH);
    } 
    else {
      Serial.println("LED off");
      digitalWrite(ledPin, LOW);
    }  
  }

  // update the characteristics of our position if they've changed by more than 1'
  if (abs(headingCharacteristic.value() - heading) > 1) {
    headingCharacteristic.writeValue(heading);  
  }
  if (abs(pitchCharacteristic.value() - pitch) > 1) {
    pitchCharacteristic.writeValue(pitch);  
  }
  if (abs(rollCharacteristic.value() - roll) > 1) {
    rollCharacteristic.writeValue(roll);  
  }
}
