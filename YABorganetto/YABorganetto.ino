#include <BounceValue.h>

void setup() {
  Serial.begin(9600);
  Serial.println("Boot starting");
  setupPressureSensor();
  setupButtons();
  setupMIDI();
}

void loop() {
  readPressureSensor();
  readButtons();
}
