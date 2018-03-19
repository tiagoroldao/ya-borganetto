const byte NUM_SELECT_PINS = 7;
const int SELECT_PINS[] = {13, 14, 15, 16, 6, 5, 4};
const int IN_PIN = 7;

void setupButtons () {
  pinMode(IN_PIN, INPUT_PULLUP);
  for (unsigned int i = 0; i < sizeof( SELECT_PINS ) / sizeof(int); i++) {
    pinMode(SELECT_PINS[i], OUTPUT);
  }
}

bool readButton (byte buttonNumber) {
  for (byte b = 0; b < NUM_SELECT_PINS; b++) {
    byte state = bitRead(buttonNumber, b);
    digitalWriteFast(SELECT_PINS[b], state);
  }
  delayMicroseconds(50);

  return digitalRead(IN_PIN);
}

void logButton (BounceValue value, int i) {
    if (value.rose()) {
      Serial.println("Button number " + String(i) + " is ON");
    }
    else if (value.fell()) {
      Serial.println("Button number " + String(i) + " is OFF");
      
    }
}
