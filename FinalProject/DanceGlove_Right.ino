//This code collects data from the onboard IMU and proximity sensor,
// as well as from the thumb/tab sensor,
// and the two flex sensors on the index and middle fingers.
// This code is meant for the right hand glove.

#include "Arduino_LSM9DS1.h" // include the libary for the IMU
#include "MadgwickAHRS.h" // include the Madgwick library
#include "Arduino_APDS9960.h" // include the library for the gesture sensor

//initalize a Madgwick filter:
Madgwick filter;
//sensor's sample rate is fixed at 119 Hz:
const float sensorRate = 119.00;

//values for orientation:
float roll = 0.0;
float pitch = 0.0;
float heading = 0.0;

//value for proximity
int proximity = 0;

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


void setup() {
  Serial.begin(9600);

  //declare pin modes
  pinMode(tap, INPUT);
  pinMode(flex1, INPUT);
  pinMode(flex2, INPUT);
  
  //attempt to start the IMU:
 if (!IMU.begin()) {
   Serial.println("Failed to initialize IMU");
    //stop here if you can't access the IMU:
  //  while(true);
  }
  // attempt to start the gesture sensor
  if (!APDS.begin()) {
  Serial.println("Error initializing APDS9960 sensor!");
    }

  //start the filter to run at the sample rate:
  filter.begin(sensorRate);

  Serial.print("proximity");
  Serial.print(",");
  Serial.print("heading");
  Serial.print(",");
  Serial.print("pitch");
  Serial.print(",");
  Serial.print("roll");
  Serial.print(",");
  Serial.print("tap");
  Serial.print(",");
  Serial.print("flex1");
  Serial.print(",");
  Serial.println("flex2");

}

void loop() {
  //values for tap and flex sensors:
  valtap = analogRead(tap);
  valflex1 = analogRead(flex1);
  valflex2 = analogRead(flex2);
  
  //values for acceleration and rotation:
  float xAcc, yAcc, zAcc;
  float xGyro, yGyro, zGyro;

  //check if the IMU and proximity sensor is ready to read
  if(IMU.accelerationAvailable() && IMU.gyroscopeAvailable() && APDS.proximityAvailable()) {
    //read accelerometer, gyrometer and proximity:
    IMU.readAcceleration(xAcc, yAcc, zAcc);
    IMU.readGyroscope(xGyro, yGyro, zGyro);
    APDS.readProximity(); // 0 => close, 255 => far, -1 => error

    //update the filter, which computes orientation:
    filter.updateIMU(xGyro, yGyro, zGyro, xAcc, yAcc, zAcc);

    //print the heading, pitch, and roll
    roll = filter.getRoll();
    pitch = filter.getPitch();
    heading = filter.getYaw();

    //print proximity
    proximity = APDS.readProximity();

    //determine value of tap
      if(valtap >= 200) { tapID = 3; //both
      }
      else if(valtap > 100 && valtap < 200) { tapID = 1; //index
      }
      else if(valtap > 10 && valtap < 100) { tapID = 2; // middle
      }
      else if(valtap < 10) { tapID = 0; //neither
      }
 }

    //print the results:
    Serial.print(proximity);
    Serial.print(",");
    Serial.print(heading);
    Serial.print(",");
    Serial.print(pitch);
    Serial.print(",");
    Serial.print(roll);
    Serial.print(",");
    Serial.print(tapID);
    Serial.print(",");
    Serial.print(valflex1);
    Serial.print(",");
    Serial.println(valflex2);

}
