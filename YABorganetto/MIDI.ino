float pressurePositiveMaxLimit = 600;
float pressurePositiveMinLimit = 90;
float pressureNegativeMaxLimit = -600;
float pressureNegativeMinLimit = -90;

float minStrength = 1;
float maxStrength = 127;

extern const byte buttonCount;

bool notesOnClosing[buttonCount];
bool notesOnOpening[buttonCount];
int notesClosing[] = {66, 69, 72, 76, 78, 81, 0, 0, 62, 65,
                      69, 83, 81, 77, 74, 71, 64, 60, 57, 54,
                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                      0, 0
                     };
int notesOpening[] = {67, 71, 74, 79, 83, 84, 0, 0, 60, 64,
                      67, 88, 84, 79, 76, 72, 62, 59, 55, 50,
                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                      0, 0
                     };

int prevStrength = 0;
int strength = 0;
bool opening = false;

void setupMIDI () {
  for (byte i = 0; i < buttonCount; i++) {
    notesOnClosing[i] = false;
    notesOnOpening[i] = false;
  }
}

void updateNoteStrength () {
  prevStrength = strength;
  float pressure;
  if (prevStrength == 0) {
    pressure = getRelativePressurePrediction(100);
  } else {
    pressure = getRelativePressure();
  }
  opening = pressure < 0;
  strength = mapNoteStrength(pressure);
}

int mapNoteStrength (float pressure) {
  if (pressure > pressurePositiveMinLimit) {
    return map(min(pressure, pressurePositiveMaxLimit), pressurePositiveMinLimit, pressurePositiveMaxLimit, minStrength, maxStrength);
  }
  else if (pressure < pressureNegativeMinLimit) {
    return map(max(pressure, pressureNegativeMaxLimit), pressureNegativeMinLimit, pressureNegativeMaxLimit, minStrength, maxStrength);
  }
  return 0;
}

void updateNotes (BounceValue values[]) {
  updateNoteStrength();

  for (byte i = 0; i < buttonCount; i++) {
    bool noteOn = strength > 0 && buttonValues[i].read();
    playNote(noteOn && opening, notesOnOpening, notesOpening, i);
    playNote(noteOn && !opening, notesOnClosing, notesClosing, i);
  }


  usbMIDI.sendControlChange(1, strength, 1);
  usbMIDI.sendAfterTouch(strength, 1);
}

void playNote (bool noteOn, bool notesOn[], int notes[], int i) {
  if (notes[i] != 0 && noteOn != notesOn[i]) {
    notesOn[i] = noteOn;
    if (notesOn[i]) {
      //      if (prevStrength == 0) {
      usbMIDI.sendNoteOn(notes[i], strength, 1);
      //      } else {
      //        usbMIDI.sendNoteOn(notes[i], strength, 1);
      //      }
    }
    else {
      usbMIDI.sendNoteOff(notes[i], 127, 1);
    }
  }
}
