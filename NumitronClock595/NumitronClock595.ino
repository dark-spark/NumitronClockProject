#include <TimerOne.h>
#include <TimerThree.h>

const int ledPin = 6;
int s, s1, m = 8, m1 = 1, h, h1 = 2;
boolean stringComplete = false;
String inputString;
int latchPin = 43;
int clockPin = 44;
int dataPin = 45;
byte digits[11] = {
  246, 192, 174, 236, 216, 124, 126, 224, 254, 252, 1};
int shiftRegisters = 2;
int value[2];
int ticker;
int interrupt;
boolean tick;

void sendDigits(int *number, int registers, boolean comma) {
  digitalWrite(latchPin, LOW);
  for (int reg = 0; reg < registers; reg++) {
    int digit = number[reg];
    for (byte bitMask = 128; bitMask > 0; bitMask >>=1 ) {
      digitalWrite(clockPin, LOW);
      digitalWrite(dataPin, digits[digit] & bitMask ? HIGH : LOW);
      digitalWrite(clockPin, HIGH);
    }
  }
  digitalWrite(latchPin, HIGH);
}

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  Timer1.initialize(1000000);
  Timer1.attachInterrupt(count);
  Timer3.initialize(1500000);
  Timer3.attachInterrupt(increment);

  /*
  for (int j = 0; j < 99; j++) {
   digitalWrite(latchPin, LOW);
   shiftOut(dataPin, clockPin, LSBFIRST, j);
   digitalWrite(latchPin, HIGH);
   delay(1000);
   }
   */
}

void formatArray() {
  if (tick) {
    value[0] = m;
    value[1] = m1;
  }
  else {
    value[0] = h;
    value[1] = h1;
  }
}

void loop() {
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

void increment() {
  digitalWrite(ledPin, HIGH);
  tick = tick ? false : true;
  formatArray();
  sendDigits(value, shiftRegisters, tick);
  updateDisplay();
  digitalWrite(ledPin, LOW);
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





