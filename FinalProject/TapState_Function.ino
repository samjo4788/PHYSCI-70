//This code should produce a change in tap state and a duration 
//reading for a circuit with two switches. 
//"index" has a 4.7kOhm resistor, 
//"middle" has a 10kOhm resistor; 
//thumb has voltage divider of 1kOhm.

unsigned long currentMillis;
long previousMillis = 0;
const unsigned long period = 1000;  //the value is a number of milliseconds
int tap = A3; //define pin A3 as the tap sensing pin
int tapValue = 0;
int tapState = 0;
long prevtapState = 0;

void setup() {
  Serial.begin(9600);
  //pinMode(tap, INPUT);
}

void loop() {
  currentMillis = millis();
  tapValue = analogRead(tap);
  tapState = myTapStateFunction (tapValue);

  if (tapState != prevtapState) {
    Serial.print(tapState);
    Serial.print('\t');
    Serial.println(currentMillis - previousMillis);
    prevtapState = tapState;
    previousMillis = currentMillis;
  }

}


int myTapStateFunction (int x) {
  int result;
  if (x >= 100) {
    result = 1; //index finger has made contact
  }

  else if (x >= 80 && x <= 99) {
    result = 2; //middle finger has made contact
  }
  else if (x <= 30 && x >= 10) {
    result = 3; //both fingers are making contact
  }

  else if (x <= 9) {
    result = 0; //neither finger is making contact
  }
  return result;
}
