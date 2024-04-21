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
  // serial stuffs
  Serial.begin(115200);
  while (!Serial) {}

  // fan pwm
  pinMode(FAN_PWM_PIN, OUTPUT);
  analogWrite(FAN_PWM_PIN, 0);

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
  bool expectCommand = true;
  char commandName = 0;
  char commandArg = 0;
  while (Serial.available() > 0) {
    char c = Serial.read();
    Serial.print("[ser] ");
    Serial.print(c);
    if (c == '\n') {
      Serial.println(" -> newline");
      if (!expectCommand) {
        enactCommand(commandName, commandArg);
      }
      expectCommand = true;
    } else if (expectCommand) {
      Serial.println(" -> name");
      commandName = c;
      expectCommand = false;
    } else {
      Serial.println(" -> arg");
      commandArg = c;
    }
  }
}

int currentRow = 0;

void enactCommand(char name, char arg) {
  Serial.print("[cmd] ");
  Serial.print(name);
  Serial.println((int)arg);
  switch (name) {
    case 'f':
      analogWrite(FAN_PWM_PIN, (int)arg);
      return;
    case 'r':
      currentRow = (int)arg;
      return;
    case 'd':
      lc.setRow(0, currentRow, arg);
      return;
    case 'c':
      lc.clearDisplay(0);
      return;
    case 'i':
      lc.setIntensity(0, (int)arg);
      return;
  }
}
