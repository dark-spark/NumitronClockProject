
const int ledPin = 6;
int s, s1, m, m1, h, h1;
boolean stringComplete = false;
String inputString;
int latchPin = 43;
int clockPin = 44;
int dataPin = 45;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  
  /*
  for (int j = 0; j < 99; j++) {
    //ground latchPin and hold low for as long as you are transmitting
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, j);
    digitalWrite(latchPin, HIGH);
    delay(1000);
  }
  */
}

void loop() {
  delay(1000);
  count();
  updateDisplay();
  while (Serial.available()) {
    char inChar = (char)Serial.read(); 
    if (inChar == '.') {
      stringComplete = true;
      break;
    }
    inputString += inChar;
  }
  if(stringComplete) {
    s = inputString[5] - 48;
    s1 = inputString[4] - 48;
    m = inputString[3] - 48;
    m1 = inputString[2] - 48;
    h = inputString[1] - 48;
    h1 = inputString[0] - 48;
    inputString = "";
    stringComplete = false;  
  }
}

void updateDisplay() {
  Serial.print(h1);
  Serial.print(h);
  Serial.print(":");
  Serial.print(m1);
  Serial.print(m);
  Serial.print(":");
  Serial.print(s1);
  Serial.println(s);

}

void count() {
  s++;
  if(s > 9) {
    s = 0;
    s1++;
  }
  if(s1 > 5) {
    s1 = 0;
    m++;
  }
  if(m > 9) {
    m = 0;
    m1++;
  }
  if(m1 > 5) {
    m1 = 0;
    h++;
  }
  if(h > 9) {
    h = 0;
    h1++;
  }
  if((h1*10)+h > 23) {
    h = 0;
    h1 = 0;
  }
}

