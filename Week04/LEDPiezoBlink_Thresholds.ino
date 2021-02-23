const int piezoSensor = A5; // analog pin in piezo is connected to
int sensorReading = 0; //variable to store value read from sensor
int led1 = 13; // the pin the LED is connected to
int led2 = 12;
int led3 = 11;

void setup() {
  pinMode(led1, OUTPUT); // Declare the LED as an output
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(A0, OUTPUT);  //This will be GND for the potentiometer
  pinMode(A4, OUTPUT);  //This will be 3.3V for the pot.
  digitalWrite(A0, LOW);
  digitalWrite(A4, HIGH);
  Serial.begin(9600); //start serial monitor window
  digitalWrite(led1, LOW); //start with LEDs off
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
}

void loop() {
  sensorReading = analogRead(piezoSensor);
  Serial.println(sensorReading);
  if(sensorReading > 0 && sensorReading <=300) {
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  int pot_value = analogRead(A2); // pot wiper is on A2
  int delay_level = map(pot_value, 0, 1023, 2000, 0); // map motor level to delay period
  digitalWrite(led1, HIGH); // Turn the LED on
  delay(delay_level); // wait for a time dependent on the pot level
  digitalWrite(led1, LOW); //Turn the LED off
  delay(delay_level); // wait for a time dependent on the pot level

  delay(10);
  }

  else if (sensorReading > 300 && sensorReading <= 600) {
  digitalWrite(led1, LOW);
  digitalWrite(led3, LOW);
  int pot_value = analogRead(A2); // pot wiper is on A2
  int delay_level = map(pot_value, 0, 1023, 2000, 0); // map motor level to delay period
  digitalWrite(led2, HIGH); // Turn the LED on
  delay(delay_level); // wait for a time dependent on the pot level
  digitalWrite(led2, LOW); //Turn the LED off
  delay(delay_level); // wait for a time dependent on the pot level

  delay(10);
  }

else if (sensorReading > 600) {
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  int pot_value = analogRead(A2); // pot wiper is on A2
  int delay_level = map(pot_value, 0, 1023, 2000, 0); // map motor level to delay period
  digitalWrite(led3, HIGH); // Turn the LED on
  delay(delay_level); // wait for a time dependent on the pot level
  digitalWrite(led3, LOW); //Turn the LED off
  delay(delay_level); // wait for a time dependent on the pot level

  delay(10);
  }

else {
    digitalWrite(led1, LOW); //Turn all LEDs off
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);

    delay(10);
  }
}
