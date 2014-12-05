#include <LedControl.h>

// inputs: DIN pin, CLK pin, LOAD pin. number of chips
LedControl mydisplay = LedControl(45, 44, 43, 1);
const int ledPin = 6;

void setup() {
  pinMode(ledPin, OUTPUT);
  mydisplay.shutdown(0, false);  // turns on display
  mydisplay.setIntensity(0, 15); // 15 = brightest
  mydisplay.setDigit(0, 0, 9, false);
  mydisplay.setDigit(0, 1, 8, false);
  mydisplay.setDigit(0, 2, 7, false);
  mydisplay.setDigit(0, 3, 6, false);
  mydisplay.setDigit(0, 4, 5, true);
  mydisplay.setDigit(0, 5, 4, false);
  
}

void loop() {
  digitalWrite(ledPin, HIGH);   // set the LED on
  mydisplay.setDigit(0, 0, 9, false);
  delay(1000);                  // wait for a second
  digitalWrite(ledPin, LOW);    // set the LED off
  mydisplay.setDigit(0, 0, 1, false);
  delay(1000); 
}
