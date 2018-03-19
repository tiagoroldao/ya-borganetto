const int PRESSURE_POSITIVE_MAX_LIMIT = 600;
const int PRESSURE_POSITIVE_MIN_LIMIT = 90;
const int PRESSURE_NEGATIVE_MAX_LIMIT = -600;
const int PRESSURE_NEGATIVE_MIN_LIMIT = -90;

const int PRESSURE_POSITIVE_PLAY_THRESHOLD = 40;
const int PRESSURE_NEGATIVE_PLAY_THRESHOLD = -40;

const int MIN_STRENGTH = 1;
const int MAX_STRENGTH = 127;

/**
   For reference, physical organetto mapping:

                   5
               11
                   4
               12
                   3
   25  26      13
                   2
   31  112     14
                   1
   23  24      15
                   0
   29  30      10
                   16
   21  22      9
                   17
   27  28      8
                   18
               7
                   19
               6
                   20
*/

// Difference in pressure above which a new attack should be done - 0 means never re-attack
const int RE_ATTACK_DIFF = 400;

struct KeyNote {
  byte keyNumber;
  byte channel;
  byte noteOpening;
  byte noteClosing;
  bool on;
  BounceValue value;
};

const int noteCount = 41;

byte midiNote (String note, int scale) {
  int baseNote = 0;
  if (note == "C") {
    baseNote = 0;
  } else if (note == "C#") {
    baseNote = 1;
  } else if (note == "D") {
    baseNote = 2;
  } else if (note == "D#") {
    baseNote = 3;
  } else if (note == "E") {
    baseNote = 4;
  } else if (note == "F") {
    baseNote = 5;
  } else if (note == "F#") {
    baseNote = 6;
  } else if (note == "G") {
    baseNote = 7;
  } else if (note == "G#") {
    baseNote = 8;
  } else if (note == "A") {
    baseNote = 9;
  } else if (note == "A#") {
    baseNote = 10;
  } else if (note == "B") {
    baseNote = 11;
  }

  return constrain(baseNote + ((scale - 2) * 12), 0, 127);
}

KeyNote notes[noteCount];

//KeyNote notes[] = {
//  // Exterior Range
//  //  {20, 1, 0,  0,  false, new BounceValue(HIGH)},
//  {19, 1, 50, 54, false, new BounceValue(HIGH)},
//  {18, 1, 55, 57, false, new BounceValue(HIGH)},
//  {17, 1, 59, 60, false, new BounceValue(HIGH)},
//  {16, 1, 62, 64, false, new BounceValue(HIGH)},
//  {0,  1, 67, 66, false, new BounceValue(HIGH)},
//  {1,  1, 71, 69, false, new BounceValue(HIGH)},
//  {2,  1, 74, 72, false, new BounceValue(HIGH)},
//  {3,  1, 79, 76, false, new BounceValue(HIGH)},
//  {4,  1, 83, 78, false, new BounceValue(HIGH)},
//  {5,  1, 84, 81, false, new BounceValue(HIGH)},
//
//  // Interior Range
//  //  {6,  2, 0,  0,  false, new BounceValue(HIGH)},
//  //  {7,  2, 0,  0,  false, new BounceValue(HIGH)},
//  {8,  2, 60, 62, false, new BounceValue(HIGH)},
//  {9,  2, 64, 65, false, new BounceValue(HIGH)},
//  {10, 2, 67, 69, false, new BounceValue(HIGH)},
//  {15, 2, 72, 71, false, new BounceValue(HIGH)},
//  {14, 2, 76, 74, false, new BounceValue(HIGH)},
//  {13, 2, 79, 77, false, new BounceValue(HIGH)},
//  {12, 2, 84, 81, false, new BounceValue(HIGH)},
//  {11, 2, 88, 83, false, new BounceValue(HIGH)},
//
//  // Bases Interior
//  {28, 9,  45,  40, false, new BounceValue(HIGH)},  //a(m), e
//  {28, 9,  36,  43, false, new BounceValue(HIGH)},  //a(m), e
//  {28, 9,  40,  47, false, new BounceValue(HIGH)},  //a(m), e
//  {22, 10, 45, 40, false, new BounceValue(HIGH)}, // A, E
//  {30, 11, 41,  41, false, new BounceValue(HIGH)},  // f, f
//  {30, 11, 45,  45, false, new BounceValue(HIGH)},  // f, f
//  {30, 11, 36,  36, false, new BounceValue(HIGH)},  // f, f
//  {24, 12, 41, 41, false, new BounceValue(HIGH)}, // F, F
//  {112, 13, 0,  0, false, new BounceValue(HIGH)},
//  {26, 14, 0,  0, false, new BounceValue(HIGH)},
//
//  // Bases Exterior
//  {27,  3, 38,  43, false, new BounceValue(HIGH)}, // d, g
//  {27,  3, 41,  47, false, new BounceValue(HIGH)}, // d, g
//  {27,  3, 45,  38, false, new BounceValue(HIGH)}, // d, g
//  {21,  4, 38, 43, false, new BounceValue(HIGH)}, // D, G
//  {29,  5, 43,  36, false, new BounceValue(HIGH)}, // g, c
//  {29,  5, 47,  40, false, new BounceValue(HIGH)}, // g, c
//  {29,  5, 38,  43, false, new BounceValue(HIGH)}, // g, c
//  {23,  6, 43, 36, false, new BounceValue(HIGH)}, // G, C
//  {31, 7, 0,  0, false, new BounceValue(HIGH)},
//  {25,  8, 0,  0, false, new BounceValue(HIGH)},
//};

void setupNotes () {
  // Exterior Range
  notes[0] = {20, 1, 0,  0,  false, new BounceValue(HIGH)};
  notes[1] = {19, 1, 50, 54, false, new BounceValue(HIGH)};
  notes[2] = {18, 1, 55, 57, false, new BounceValue(HIGH)};
  notes[3] = {17, 1, 59, 60, false, new BounceValue(HIGH)};
  notes[4] = {16, 1, 62, 64, false, new BounceValue(HIGH)};
  notes[5] = {0,  1, 67, 66, false, new BounceValue(HIGH)};
  notes[6] = {1,  1, 71, 69, false, new BounceValue(HIGH)};
  notes[7] = {2,  1, 74, 72, false, new BounceValue(HIGH)};
  notes[8] = {3,  1, 79, 76, false, new BounceValue(HIGH)};
  notes[9] = {4,  1, 83, 78, false, new BounceValue(HIGH)};
  notes[10] = {5,  1, 84, 81, false, new BounceValue(HIGH)};

  // Interior Range
  notes[11] = {6,  2, 0,  0,  false, new BounceValue(HIGH)};
  notes[12] = {7,  2, 0,  0,  false, new BounceValue(HIGH)};
  notes[13] = {8,  2, 60, 62, false, new BounceValue(HIGH)};
  notes[14] = {9,  2, 64, 65, false, new BounceValue(HIGH)};
  notes[15] = {10, 2, 67, 69, false, new BounceValue(HIGH)};
  notes[16] = {15, 2, 72, 71, false, new BounceValue(HIGH)};
  notes[17] = {14, 2, 76, 74, false, new BounceValue(HIGH)};
  notes[18] = {13, 2, 79, 77, false, new BounceValue(HIGH)};
  notes[19] = {12, 2, 84, 81, false, new BounceValue(HIGH)};
  notes[20] = {11, 2, 88, 83, false, new BounceValue(HIGH)};

  // Bases Interior
  notes[21] = {28, 9,  45,  40, false, new BounceValue(HIGH)};  //a(m), e
  notes[22] = {28, 9,  36,  43, false, new BounceValue(HIGH)};  //a(m), e
  notes[23] = {28, 9,  40,  47, false, new BounceValue(HIGH)};  //a(m), e
  notes[24] = {22, 10, 45, 40, false, new BounceValue(HIGH)}; // A, E
  notes[25] = {30, 11, 41,  41, false, new BounceValue(HIGH)};  // f, f
  notes[26] = {30, 11, 45,  45, false, new BounceValue(HIGH)};  // f, f
  notes[27] = {30, 11, 36,  36, false, new BounceValue(HIGH)};  // f, f
  notes[28] = {24, 12, 41, 41, false, new BounceValue(HIGH)}; // F, F
  notes[29] = {112, 13, 0,  0, false, new BounceValue(HIGH)};
  notes[30] = {26, 14, 0,  0, false, new BounceValue(HIGH)};

  // Bases Exterior
  notes[31] = {27,  3, 38,  43, false, new BounceValue(HIGH)}; // d, g
  notes[32] = {27,  3, 41,  47, false, new BounceValue(HIGH)}; // d, g
  notes[33] = {27,  3, 45,  38, false, new BounceValue(HIGH)}; // d, g
  notes[34] = {21,  4, 38, 43, false, new BounceValue(HIGH)}; // D, G
  notes[35] = {29,  5, 43,  36, false, new BounceValue(HIGH)}; // g, c
  notes[36] = {29,  5, 47,  40, false, new BounceValue(HIGH)}; // g, c
  notes[37] = {29,  5, 38,  43, false, new BounceValue(HIGH)}; // g, c
  notes[38] = {23,  6, 43, 36, false, new BounceValue(HIGH)}; // G, C
  notes[39] = {31, 7, 0,  0, false, new BounceValue(HIGH)};
  notes[40] = {25,  8, 0,  0, false, new BounceValue(HIGH)};
}

int prevStrength = 0;
int strength = 0;
bool opening = false;
bool prevOpening = false;
float pressureRead = 0;
float prevPressureRead = 0;

void updateNoteStrength () {
  prevStrength = strength;
  prevOpening = opening;
  prevPressureRead = pressureRead;
  pressureRead = getRelativePressure();

  if (pressureRead > PRESSURE_POSITIVE_PLAY_THRESHOLD || pressureRead < PRESSURE_NEGATIVE_PLAY_THRESHOLD) {
    float prediction = getRelativePressurePrediction(50);
    if ((prediction > 0 && pressureRead < 0) || (prediction < 0 && pressureRead > 0)) {
      // avoid predicting too soon (results in lower initial values for notes)
      pressureRead = 0;
    }
    else {
      pressureRead = prediction;
    }
  } else {
    pressureRead = 0;
  }

  opening = pressureRead < 0;
  strength = mapNoteStrength(getRelativePressurePrediction(50));
}

int mapNoteStrength (float pressure) {
  if (pressureRead > PRESSURE_POSITIVE_MIN_LIMIT) {
    return map(min(pressureRead, PRESSURE_POSITIVE_MAX_LIMIT), PRESSURE_POSITIVE_MIN_LIMIT, PRESSURE_POSITIVE_MAX_LIMIT, MIN_STRENGTH, MAX_STRENGTH);
  }
  else if (pressureRead < PRESSURE_NEGATIVE_MIN_LIMIT) {
    return map(max(pressureRead, PRESSURE_NEGATIVE_MAX_LIMIT), PRESSURE_NEGATIVE_MIN_LIMIT, PRESSURE_NEGATIVE_MAX_LIMIT, MIN_STRENGTH, MAX_STRENGTH);
  }
  return 0;
}

void updateNotes () {
  updateNoteStrength();
  bool foleChange = opening != prevOpening;

  if (foleChange) {
    for (byte i = 0; i < noteCount; i++) {
      if (notes[i].on) {
        usbMIDI.sendNoteOff(
          prevOpening ? notes[i].noteOpening : notes[i].noteClosing,
          127,
          notes[i].channel);
        notes[i].on = false;
        //Serial.println(prevOpening ? notes[i].noteOpening : notes[i].noteClosing);
      }
    }
  }

  for (byte i = 0; i < noteCount; i++) {
    int note = opening ? notes[i].noteOpening : notes[i].noteClosing;
    if (note != 0) {
      notes[i].value.update(!readButton (notes[i].keyNumber));
      logButton (notes[i].value, notes[i].keyNumber);
      bool buttonPressed = notes[i].value.read();

      if (!notes[i].on && strength > 0 && buttonPressed) {
        usbMIDI.sendNoteOn(note, strength, notes[i].channel);
        notes[i].on = true;
      }
      else if (notes[i].on) {
        //Serial.println("0 127 -127 " + String(RE_ATTACK_DIFF) + " " + String(abs(pressureRead - prevPressureRead)));
        if (pressureRead == 0 || !buttonPressed) {
          usbMIDI.sendNoteOff(note, 127, notes[i].channel);
          notes[i].on = false;
        }
        else if (abs(pressureRead - prevPressureRead) > RE_ATTACK_DIFF) {
          usbMIDI.sendNoteOff(note, 127, notes[i].channel);
          usbMIDI.sendNoteOn(note, strength, notes[i].channel);
        }
      }
    }
  }


  usbMIDI.sendControlChange(1, strength, 1);
  usbMIDI.sendAfterTouch(strength, 1);
}
