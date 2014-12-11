#include <TimerOne.h>
#include <TimerThree.h>
#include <Wire.h>
#include <Time.h>
#include <DS1307RTC.h>
#include <Servo.h>

const int ledPin = 6;
int s, s1, m = 8, m1 = 1, h, h1 = 2;
boolean stringComplete = false;
String inputString;
int interruptPin = 42;
int latchPin = 43;
int clockPin = 44;
int dataPin = 45;
int analogPin = 41;
int val;
byte digits[11] = {
  246, 192, 174, 236, 216, 124, 126, 224, 254, 252, 1};
int shiftRegisters = 2;
int value[2];
int ticker;
int interrupt;
boolean tick;
const char *monthName[12] = {
  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

Servo fan;
tmElements_t tm;

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
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(6, count, RISING);
  Timer3.initialize(1500000);
  Timer3.attachInterrupt(increment);

  bool parse=false;
  bool config=false;

  if (RTC.read(tm)) {
    s= firstDigit(tm.Second);
    s1= secondDigit(tm.Second);
    m= firstDigit(tm.Minute);
    m1= secondDigit(tm.Minute);
    h= firstDigit(tm.Hour);
    h1= secondDigit(tm.Hour);
  }
  
  fan.attach(40);

  /*
  // get the date and time the compiler was run
   if (getDate(__DATE__) && getTime(__TIME__)) {
   parse = true;
   // and configure the RTC with this info
   if (RTC.write(tm)) {
   config = true;
   }
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
  
  val = analogRead(analogPin); 
  val = map(val, 0, 1023, 0, 180); 
    fan.write(val); 
    delay(100);
  
  
    
  while (Serial.available()) {
    char inChar = (char)Serial.read(); 
    if (inChar == '.') {
      stringComplete = true;
      break;
    }
    inputString += inChar;
  }

  if(stringComplete) {
    if (RTC.read(tm)) {
      Serial.print("Ok, Time = ");
      print2digits(tm.Hour);
      Serial.write(':');
      print2digits(tm.Minute);
      Serial.write(':');
      print2digits(tm.Second);
      Serial.print(", Date (D/M/Y) = ");
      Serial.print(tm.Day);
      Serial.write('/');
      Serial.print(tm.Month);
      Serial.write('/');
      Serial.print(tmYearToCalendar(tm.Year));
      Serial.println();
      s= firstDigit(tm.Second);
      s1= secondDigit(tm.Second);
      m= firstDigit(tm.Minute);
      m1= secondDigit(tm.Minute);
      h= firstDigit(tm.Hour);
      h1= secondDigit(tm.Hour);
    }
    inputString = "";
    stringComplete = false;  
  }  
}

void increment() {
  //  digitalWrite(ledPin, HIGH);
  tick = tick ? false : true;
  formatArray();
  sendDigits(value, shiftRegisters, tick);
  updateDisplay();
  //  digitalWrite(ledPin, LOW);
}

void print2digits(int number) {
  if (number >= 0 && number < 10) {
    Serial.write('0');
  }
  Serial.print(number);
}

int firstDigit(int number) {
  if (number > 9) {
    return number % 10;
  }
}

int secondDigit(int number) {
  if(number > 9) {
    return floor(number / 10);
  } 
  else {
    return 0;
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

bool getTime(const char *str)
{
  int Hour, Min, Sec;

  if (sscanf(str, "%d:%d:%d", &Hour, &Min, &Sec) != 3) return false;
  tm.Hour = Hour;
  tm.Minute = Min;
  tm.Second = Sec;
  return true;
}

bool getDate(const char *str)
{
  char Month[12];
  int Day, Year;
  uint8_t monthIndex;

  if (sscanf(str, "%s %d %d", Month, &Day, &Year) != 3) return false;
  for (monthIndex = 0; monthIndex < 12; monthIndex++) {
    if (strcmp(Month, monthName[monthIndex]) == 0) break;
  }
  if (monthIndex >= 12) return false;
  tm.Day = Day;
  tm.Month = monthIndex + 1;
  tm.Year = CalendarYrToTm(Year);
  return true;
}




