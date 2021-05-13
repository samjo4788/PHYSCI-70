//This sketch produces IMU and proximity data for the Arduino Nano 33 BLE Sense

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

void setup() {
  Serial.begin(9600);
  //attempt to start the IMU:
 if (!IMU.begin()) {
   Serial.println("Failed to initialize IMU");
  }
  // attempt to start the gesture sensor
  if (!APDS.begin()) {
  Serial.println("Error initializing APDS9960 sensor!");
    }

  //start the filter to run at the sample rate:
  filter.begin(sensorRate);

}

void loop() {
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
  }

    //print the results:
    Serial.print(proximity);
    Serial.print(",");
    Serial.print(heading);
    Serial.print(",");
    Serial.print(pitch);
    Serial.print(",");
    Serial.println(roll);

}
