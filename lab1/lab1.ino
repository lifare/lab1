#include <Arduino.h>
#include <MD_TCS230.h>
#include "pitches.h"
#include "buzzer.h"

#define  S0_OUT  2
#define  S1_OUT  3
#define  S2_OUT  4
#define  S3_OUT  5
#define PIN_BUZZER_1 6
#define PIN_BUZZER_2 7
enum {
  white = 0,
  red = 1,
  green = 2,
  blue = 3,
  red_and_blue = 4,
  red_and_green = 5,
  green_and_blue = 6,
  black = 7,
  unknow = 8
};

MD_TCS230 *colorSensor;
Buzzer buzzer(PIN_BUZZER_1,PIN_BUZZER_2);
int notes[] = {NOTE_C1, NOTE_C2, NOTE_C3, NOTE_C4, NOTE_C5, NOTE_C6, NOTE_C7, NOTE_C8, NOTE_D8};
double durations[] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
int melodyLength = 9;

void setup()
{
    Serial.begin(115200);
    Serial.println("Started!");

    sensorData whiteCalibration;
    whiteCalibration.value[TCS230_RGB_R] = 131610;
    whiteCalibration.value[TCS230_RGB_G] = 106380;
    whiteCalibration.value[TCS230_RGB_B] = 144050;
    
    sensorData blackCalibration;
    blackCalibration.value[TCS230_RGB_R] = 14330;
    blackCalibration.value[TCS230_RGB_G] = 11500;
    blackCalibration.value[TCS230_RGB_B] = 15710;


    colorSensor->begin();
    colorSensor->setDarkCal(&blackCalibration);
    colorSensor->setWhiteCal(&whiteCalibration);
    
    buzzer.setMelody(notes, durations, melodyLength);
    buzzer.turnSoundOn();
}

void loop() 
{
    colorSensor = new MD_TCS230(S2_OUT, S3_OUT, S0_OUT, S1_OUT);
    colorData rgb;
    colorSensor->read();
    while (!colorSensor->available());
    colorSensor->getRGB(&rgb);
    print_rgb(rgb);
    delete colorSensor;
    int color = getColor(rgb);
    buzzer.playNote(color);
}

int getColor(colorData rgb){
  int r = rgb.value[TCS230_RGB_R];
  int g = rgb.value[TCS230_RGB_G];
  int b = rgb.value[TCS230_RGB_B];
  int dif = 10;
  int dominante_color = max(max(r,g),b);
  bool check_black = 255 - dominante_color < 30 && dominante_color - r < 15 && dominante_color - g < 15 && dominante_color - b < 15;
  if (dominante_color < 10)
    return white - dif;
  if (check_black)
    return black;
  if (dominante_color - r < 15 && dominante_color - g < 15)
    return red_and_green;
  if (dominante_color - r < 15 && dominante_color - b < 15)
    return red_and_blue;
  if (dominante_color - b < 15 && dominante_color - g < 15)
    return green_and_blue;
  if (dominante_color == r)
    return red;
  if (dominante_color == g)
    return green;
  if (dominante_color == b)
    return blue;
  return unknow;
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
