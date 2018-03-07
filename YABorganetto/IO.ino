const byte NUM_SELECT_PINS = 7;
const int SELECT_PINS[] = {13, 14, 15, 16, 6, 5, 4};
const int IN_PIN = 7;

const byte buttonCount = 32;
BounceValue buttonValues[buttonCount];

void setupButtons () {
  pinMode(IN_PIN, INPUT_PULLUP);
  for (unsigned int i = 0; i < sizeof( SELECT_PINS ) / sizeof(int); i++) {
    pinMode(SELECT_PINS[i], OUTPUT);
  }

  for (byte i = 0; i < buttonCount; i++) {
    buttonValues[i] = new BounceValue(HIGH);

    for (byte b = 0; b < NUM_SELECT_PINS; b++) {
      byte state = bitRead(i, b);
      digitalWrite(SELECT_PINS[b], state);
    }
    buttonValues[i].update(digitalRead(IN_PIN));
  }
}

bool prevVal = 0;

void readButtons () {
  
  for (byte i = 0; i < buttonCount; i++) {
    for (byte b = 0; b < NUM_SELECT_PINS; b++) {
      byte state = bitRead(i, b);
      digitalWrite(SELECT_PINS[b], state);
    }
    buttonValues[i].update(digitalRead(IN_PIN));
  }


  updateNotes(buttonValues);
}

BounceValue getButton(int index) {
  return buttonValues[index];
}
