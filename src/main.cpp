#include <Arduino.h>

#include "pong.h"


void setup() {
  Wire.begin();
  initializeOLED();
  clearDisplay();
  initializeButtons();
  startScreen();
  while(checkButtons() == 0);
  clearDisplay();
  initializeObjects();
}



void loop() {
  drawFrame();
  checkEvents();
  delay(1000/50);
}

