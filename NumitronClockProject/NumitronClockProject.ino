#include <LedControl.h>

// inputs: DIN pin, CLK pin, LOAD pin. number of chips
LedControl mydisplay = LedControl(45, 44, 43, 1);
const int ledPin = 6;
int s, s1, m, m1, h, h1;
boolean stringComplete = false;
String inputString;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  mydisplay.shutdown(0, false);  // turns on display
  mydisplay.setIntensity(0, 15); // 15 = brightest
  mydisplay.setScanLimit(0, 2);
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
    s = int(inputString[5]);
    s1 = int(inputString[4]);
    m = int(inputString[3]);
    m1 = int(inputString[2]);
    h = int(inputString[1]);
    h1 = int(inputString[0]);
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
  mydisplay.setDigit(0,0,s,false);
  mydisplay.setDigit(0,1,s1,false);

  //  mydisplay.setDigit(0,2,h,false);
  //  mydisplay.setDigit(0,3,h1,false);
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
  if(h1 > 2 && h > 3) {
    h = 0;
    h1 = 0;
  }
}

