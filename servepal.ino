//We always have to include the library
#include "LedControl.h"

/*
 Now we need a LedControl to work with.
 We have only a single MAX72XX.
 */
const int DIN = 11;
const int SEL = 5;
const int CLK = 13;
LedControl lc=LedControl(DIN, CLK, SEL, 1);

/* we always wait a bit between updates of the display */
unsigned long delaytime=5;

const int FAN_PWM_PIN = 6;

void setup() {
  // fan pwm
  pinMode(FAN_PWM_PIN, OUTPUT);

  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,1);
  /* and clear the display */
  lc.clearDisplay(0);
}

void loop() {
  analogWrite(FAN_PWM_PIN, 0);
}
