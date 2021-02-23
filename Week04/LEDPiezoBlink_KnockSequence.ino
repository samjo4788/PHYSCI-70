const int piezoSensor = A5; // analog pin in piezo is connected to pin 5
const int threshold = 100;
const int timeout = 1000;
const int led1 = 13; // the pin the LED is connected to
const int led2 = 12;
const int led3 = 11;


int sensorReading = 0; //variable to store value read from sensor
int knock = 0;
int timeStart = 0;
int timeNow = 0;
int amountKockled1 = 1;
int amountKnockled2 = 2;
int amountKnockled3 = 3;


void setup() {
  pinMode(led1, OUTPUT); // Declare the LED as an output
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(A0, OUTPUT);  //This will be GND for the potentiometer
  pinMode(A4, OUTPUT);  //This will be 3.3V for the pot.
  digitalWrite(A0, LOW);
  digitalWrite(A4, HIGH);
  Serial.begin(9600); //start serial monitor window
  digitalWrite(led1, LOW); //start with all leds off
  digitalWrite(led3, LOW);
}

void loop() {
  
  while ((timeNow - timeStart < timeout) && knock < 3)  {  // check for knock till timed out or knock = 3
    sensorReading = analogRead(piezoSensor);
    timeNow = millis();
    if (sensorReading >= threshold) {                      // if knock
      knock ++;
      Serial.println(knock);
      timeStart = millis();                                // start timer;
      delay(100);                                          // wait for the knock to fade
      Serial.println("Knock");
    }
  }
  
  if ((timeNow - timeStart >= timeout) || knock == 3) { //check if timed out
    if (knock = amountKockled1){
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);
      int pot_value = analogRead(A2); // pot wiper is on A2
      int delay_level = map(pot_value, 0, 1023, 2000, 0); // map motor level to delay period
      digitalWrite(led1, HIGH); // Turn the LED on
      delay(delay_level); // wait for a time dependent on the pot level
      digitalWrite(led1, LOW); //Turn the LED off
      delay(delay_level); // wait for a time dependent on the pot level
    } else {
      if (knock = amountKnockled2) {
        digitalWrite(led1, LOW);
        digitalWrite(led3, LOW);
        int pot_value = analogRead(A2); // pot wiper is on A2
        int delay_level = map(pot_value, 0, 1023, 2000, 0); // map motor level to delay period
        digitalWrite(led2, HIGH); // Turn the LED on
        delay(delay_level); // wait for a time dependent on the pot level
        digitalWrite(led2, LOW); //Turn the LED off
        delay(delay_level); // wait for a time dependent on the pot level
      } else {
        if (knock = amountKnockled3) {
          digitalWrite(led1, LOW);
          digitalWrite(led2, LOW);
          int pot_value = analogRead(A2); // pot wiper is on A2
          int delay_level = map(pot_value, 0, 1023, 2000, 0); // map motor level to delay period
          digitalWrite(led3, HIGH); // Turn the LED on
          delay(delay_level); // wait for a time dependent on the pot level
          digitalWrite(led3, LOW); //Turn the LED off
          delay(delay_level); // wait for a time dependent on the pot level
        } else  {  // if knock is incorrect{
            digitalWrite(led1, LOW); //Turn all LEDs off
            digitalWrite(led2, LOW);
            digitalWrite(led3, LOW);
            Serial.println("False");
      }
    }
  }
  }
    knock = 0;                                             // clear knock
    timeStart = 0;                                         // reset time
    timeNow = 0;
}
  
  
