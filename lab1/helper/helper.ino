#include <Arduino.h>
#include "Wire.h"
#include "buzzer.h"
#include "pitches.h"

#define PIN_BUZZER_1 6
#define PIN_BUZZER_2 7

Buzzer buzzer(PIN_BUZZER_1,PIN_BUZZER_2);
int notes[] = {NOTE_C1, NOTE_C2, NOTE_C3, NOTE_C4, NOTE_C5, NOTE_C6, NOTE_C7, NOTE_C8, NOTE_SILENCE, NOTE_SILENCE};
double durations[] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
int melodyLength = 9;

enum {
  white = 0,
  red = 1,
  green = 2,
  blue = 3,
  yellow = 4,
  orange = 5,
  purple = 6,
  black = 7,
  unknow = 8,
  none = 9
};

int currentColor = none;
const int colorAddr = 10;

void setup()
{    
    buzzer.setMelody(notes, durations, melodyLength);
    buzzer.turnSoundOn();
    Serial.begin(115200);
    Wire.begin();
}


void loop() 
{
    currentColor = readColor();
    Serial.print(currentColor);
    Serial.print("\n");
    if (currentColor != none){
      buzzer.playNote(currentColor);
      delay(2000);
    }

};

int readColor()
{
  Wire.requestFrom(colorAddr, 1);
  if (Wire.available()) {
    return Wire.read();
  }
}
