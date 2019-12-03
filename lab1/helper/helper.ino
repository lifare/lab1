#include <Arduino.h>
#include "Wire.h"
#include "pitches.h"
#include "Colors.h"
#include <toneAC2.h>

#define PIN_BUZZER_1 6
#define PIN_BUZZER_2 7
#define BUZZER_NOTE_DURATION 100
unsigned long noteStartedMs;

int notes[] = {NOTE_A1, NOTE_B1, NOTE_C1, NOTE_D1, NOTE_E1, NOTE_F1, NOTE_G1, NOTE_GS1, NOTE_SILENCE};
double durations[] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
int melodyLength = 9;


int currentColor = unknow;
const int colorAddr = 10;

void setup()
{    
    noteStartedMs = 0;
    pinMode(PIN_BUZZER_1, OUTPUT);
    pinMode(PIN_BUZZER_2, OUTPUT);
    Serial.begin(115200);
    Wire.begin();
}


void loop() 
{
    currentColor = readColor();
    Serial.print(currentColor);
    Serial.print("\n");
    playNote(currentColor);
    delay(2000);

};

int readColor()
{
  if (Wire.available()) {
    return Wire.read();
  }
}

void playNote(int index)
{
  unsigned long duration = round(BUZZER_NOTE_DURATION*durations[index]);
  int note = notes[index];
  if (note != NOTE_SILENCE){
    toneAC2(PIN_BUZZER_1,PIN_BUZZER_2, note);
    noteStartedMs = millis();
    while(millis()- noteStartedMs < duration){
    }
  }
  noToneAC2();
}
