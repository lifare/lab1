#include <Arduino.h>
#include <MD_TCS230.h>
#include "pitches.h"
#include "buzzer.h"

#define  S0_OUT  2
#define  S1_OUT  3
#define  S2_OUT  4
#define  S3_OUT  5
#define PIN_BUZZER 6
#define white 10
#define red 11
#define green 12
#define blue 13
#define rb 14
#define rg 15
#define gb 16
#define black 17
#define unknow 18

MD_TCS230 colorSensor(S2_OUT, S3_OUT, S0_OUT, S1_OUT);
Buzzer buzzer(PIN_BUZZER);
int notes[] = {NOTE_C1, NOTE_C2, NOTE_C3, NOTE_C4, NOTE_C5, NOTE_C6, NOTE_C7, NOTE_C8, NOTE_D8};
double durations[] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
int melodyLength = 9;

void setup()
{
    Serial.begin(115200);
    Serial.println("Started!");

    sensorData whiteCalibration;
    whiteCalibration.value[TCS230_RGB_R] = 0;
    whiteCalibration.value[TCS230_RGB_G] = 0;
    whiteCalibration.value[TCS230_RGB_B] = 0;

    sensorData blackCalibration;
    blackCalibration.value[TCS230_RGB_R] = 0;
    blackCalibration.value[TCS230_RGB_G] = 0;
    blackCalibration.value[TCS230_RGB_B] = 0;

    colorSensor.begin();
    colorSensor.setDarkCal(&blackCalibration);
    colorSensor.setWhiteCal(&whiteCalibration);
    
    buzzer.setMelody(notes, durations, melodyLength);
    buzzer.turnSoundOn();
}

void loop() 
{
    colorData rgb;
    colorSensor.read();
    while (!colorSensor.available());
    colorSensor.getRGB(&rgb);
    print_rgb(rgb);
    int index = getIndex(rgb);
    buzzer.playNote(index);
}

int getIndex(color Data rgb){
  int r = rgb.value[TCS230_RGB_R];
  int g = rgb.value[TCS230_RGB_G];
  int b = rgb.value[TCS230_RGB_B];
  int dif = 10;
  dominante_color = max(max(r,g),b);
  if (dominante_color < 10)
    return white - dif;
  if (255 - dominante_color < 30 && dominante_color - r < 15 && dominante_color - g < 15 && dominante_color - b < 15)
    return black - dif;
  if (dominante_color - r < 15 && dominante_color - g < 15)
    return rg - dif;
  if (dominante_color - r < 15 && dominante_color - b < 15)
    return rb - dif;
  if (dominante_color - b < 15 && dominante_color - g < 15)
    return bg - dif;
  if (dominante_color == r)
    return red - dif;
  if (dominante_color == g)
    return green - dif;
  if (dominante_color == b)
    return blue - dif;
  return unknow - dif;
}

void print_rgb(colorData rgb)
{
  Serial.print(rgb.value[TCS230_RGB_R]);
  Serial.print(" ");
  Serial.print(rgb.value[TCS230_RGB_G]);
  Serial.print(" ");
  Serial.print(rgb.value[TCS230_RGB_B]);
  Serial.println();
}
