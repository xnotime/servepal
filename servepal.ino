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

const int MIN_COMMAND_TIME = 10000;

long lastCommandTime = -MIN_COMMAND_TIME;

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

  // say that we're ready
  Serial.println("R");
}

bool wasTimedOut = true;

void loop() {
  checkTimeouts();
  bool expectCommand = true;
  char commandName = 0;
  char commandArg = 0;
  while (Serial.available() > 0) {
    char c = Serial.read();
    if (c == '\n') {
      enactCommand(commandName, commandArg);
      expectCommand = true;
    } else if (expectCommand) {
      commandName = c;
      expectCommand = false;
    } else {
      commandArg = c;
    }
    delay(10);
  }
}

void checkTimeouts() {
  Serial.print(millis());
  Serial.print(" >= ");
  Serial.print(lastCommandTime);
  Serial.print(" + ");
  Serial.println(MIN_COMMAND_TIME);
  if (millis() >= (lastCommandTime + MIN_COMMAND_TIME)) {
    lc.setRow(0, 0, 0b10000001);
    lc.setRow(0, 1, 0b01000010);
    lc.setRow(0, 2, 0b00100100);
    lc.setRow(0, 3, 0b00011000);
    lc.setRow(0, 4, 0b00011000);
    lc.setRow(0, 5, 0b00100100);
    lc.setRow(0, 6, 0b01000010);
    lc.setRow(0, 7, 0b10000001);
    wasTimedOut = true;
  } else if (wasTimedOut) {
    lc.clearDisplay(0);
    wasTimedOut = false;
  }
}

int currentRow = 0;

void enactCommand(char name, char arg) {
  lastCommandTime = millis();
  Serial.println("y");
  switch (name) {
    case 'w':
      return;
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
