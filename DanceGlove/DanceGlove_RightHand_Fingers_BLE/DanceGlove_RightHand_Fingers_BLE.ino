//This code collects data from the thumb/tab sensor,
// and the two flex sensors on the index and middle fingers.
// This code is meant for the right hand glove for the fingers


#include "ArduinoBLE.h" //include the BLE library

//values for tap sensor
const int tap = A2; // pin assigned to read taps via thumb
int valtap = 0;
int tapID = 0;

//values for flex sensors
const int flex1 = A4; // pin assigned to read flexing of index finger
const int flex2 = A6; // pin assigned to read flexing of middle finger
int valflex1 = 0;
int valflex2 = 0;
int f1ID = 0;
int f2ID = 0;


//BLE Service
BLEService danceService("19B10010-E8F2-537E-4F6C-D104768A1214"); // create service
// a characteristic for each of the accelerometer values x, y, z
BLEIntCharacteristic tapIDCharacteristic("19B10013-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify);
BLEIntCharacteristic valflex1Characteristic("19B10014-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify);
BLEIntCharacteristic valflex2Characteristic("19B10015-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify);

void setup() {
  Serial.begin(9600);

  //declare pin modes
  pinMode(tap, INPUT);
  pinMode(flex1, INPUT);
  pinMode(flex2, INPUT);

  //attempt to start BLE:
  if (!BLE.begin()) {
  Serial.println("starting BLE failed!");
  }

  BLE.setLocalName("DanceGlove_Fingers");
  BLE.setAdvertisedService(danceService);
  danceService.addCharacteristic(tapIDCharacteristic);
  danceService.addCharacteristic(valflex1Characteristic);
  danceService.addCharacteristic(valflex2Characteristic);
 
  // add the service
  BLE.addService(danceService);

  // init all values to 0
  tapIDCharacteristic.writeValue(0);
  valflex1Characteristic.writeValue(0);
  valflex2Characteristic.writeValue(0);

  BLE.advertise();
  Serial.println("Bluetooth device active, waiting for connections...");
 

}

void loop() {

  //poll for BLE events
  BLE.poll();
  

  //variable to tell if data was requested over serial;
  bool ping = false;

  //values for tap and flex sensors:
  valtap = analogRead(tap);
  valflex1 = analogRead(flex1);
  valflex2 = analogRead(flex2);

  //determine value of tap
  if(valtap >= 200) { tapID = 3; //both
  }
  else if(valtap > 100 && valtap < 200) { tapID = 1; //index
  }
  else if(valtap > 10 && valtap < 100) { tapID = 2; // middle
  }
  else if(valtap < 10) { tapID = 0; //neither
  }


  //if you get a byte in the serial port, send latest data:
  while(Serial.available()) {
    Serial.read(); 
    ping = true; 
    }
  if (ping) {

    //print the results:
    Serial.print(tapID);
    Serial.print(",");
    Serial.print(valflex1);
    Serial.print(",");
    Serial.println(valflex2);

    }

  //update the characteristics if they have changed by more than 1:
    if (abs(tapIDCharacteristic.value() - tapID) > 1) {
    tapIDCharacteristic.writeValue(tapID);  
    }
    if (abs(valflex1Characteristic.value() - valflex1) > 1) {
    valflex1Characteristic.writeValue(valflex1);  
    }
    if (abs(valflex2Characteristic.value() - valflex2) > 1) {
    valflex2Characteristic.writeValue(valflex2);  
    }

}
