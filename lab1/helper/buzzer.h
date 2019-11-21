#include <toneAC2.h>
#include "pitches.h"
#pragma once
#define BUZZER_NOTE_DURATION 100

class Buzzer
{
public:
    Buzzer(int _pin1, int _pin2)
    {
        pin1 = _pin1;
        pin2 = _pin2;
        pinMode(pin1, OUTPUT);
        pinMode(pin2, OUTPUT);
        isEnabled = false;
        currentNote = 0;
        noteStartedMs = 0;

        notes = 0;
        durations = 0;
        melodyLength = 0;
    }

    void turnSoundOn()
    {
        isEnabled = true;
        currentNote = 0;
        noteStartedMs = 0;
    }

    void turnSoundOff()
    {
        isEnabled = false;
        currentNote = 0;
        noteStartedMs = 0;
        noToneAC2();
    }

    void setMelody(int _notes[], double _durations[], int _melodyLength)
    {
        notes = _notes;
        durations = _durations;
        melodyLength = _melodyLength;
    }

    void playSound()
    {
        if (!isEnabled)
            return;

        unsigned long duration = round(BUZZER_NOTE_DURATION*durations[currentNote]);
        if ((millis() - noteStartedMs) > duration)
        {
            int note = notes[currentNote];
            
            if (note == NOTE_SILENCE)
                noToneAC2();
            else 
                toneAC2(pin1,pin2, notes[currentNote]);

            noteStartedMs = millis();
            currentNote = (currentNote + 1)%melodyLength;
        }
    }

    void playNote(int index)
    {
      if (!isEnabled)
        return;
      unsigned long duration = round(BUZZER_NOTE_DURATION*durations[currentNote]);
      currentNote = index;
      int note = notes[currentNote];
      if (note != NOTE_SILENCE){
        toneAC2(pin1,pin2, note);
        noteStartedMs = millis();
        while(millis()- noteStartedMs < duration){
        }
      }
      else
        noToneAC2();
      noToneAC2();
    }

private:
    int pin1;
    int pin2;
    bool isEnabled;

    int currentNote;
    unsigned long noteStartedMs;

    int* notes;
    double* durations;
    int melodyLength;
};
