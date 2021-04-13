/*
  Arduino LSM9DS1 - Simple Accelerometer * Simple Gyroscope

  This example reads the acceleration and gyroscope values from the LSM9DS1
  sensor and continuously prints them to the Serial Monitor
  or Serial Plotter. This combines the SimpleAccelerometer Sketch and the SimpleGyroscope Sketch

  The circuit:
  - Arduino Nano 33 BLE Sense

  created 10 Jul 2019
  by Riccardo Rizzo

  This example code is in the public domain.
*/

#include <Arduino_LSM9DS1.h>

#include "MadgwickAHRS.h"
 
// initialize a Madgwick filter:
Madgwick filter;
// sensor's sample rate is fixed at 104 Hz:
const float sensorRate = 119.00;
 
// values for orientation:
float roll = 0.0;
float pitch = 0.0;
float heading = 0.0;


void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Started");

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println(" Hz");
  Serial.println();
  Serial.print("Gyroscope sample rate = ");
  Serial.print(IMU.gyroscopeSampleRate());
  Serial.println(" Hz");
  Serial.println();


  // start the filter to run at the sample rate:
  filter.begin(sensorRate);
}

void loop() {
  float xA, yA, zA;
  float xG, yG, zG;

  if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable()) {
    IMU.readAcceleration(xA, yA, zA);
    IMU.readGyroscope(xG, yG, zG);

    // update the filter, which computes orientation:
    filter.updateIMU(xG, yG, zG, xA, yA, zA);
 
    // print the heading, pitch and roll
    roll = filter.getRoll();
    pitch = filter.getPitch();
    heading = filter.getYaw();
  }
 
  // if you get a byte in the serial port,
  // send the latest heading, pitch, and roll:
  if (Serial.available()) {
    char input = Serial.read();
    Serial.print(heading);
    Serial.print(",");
    Serial.print(pitch);
    Serial.print(",");
    Serial.println(roll);
  }
}
