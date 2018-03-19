#include <BounceValue.h>

void setup() {
  Serial.begin(9600);
  Serial.println("Boot starting");
  setupPressureSensor();
  setupButtons();
  setupNotes();
}

void loop() {
  readPressureSensor();
  updateNotes();
}
