#include <Arduino.h>
#include <MD_TCS230.h>
#include "pitches.h"
#include "buzzer.h"

#define  S0_OUT  2
#define  S1_OUT  3
#define  S2_OUT  4
#define  S3_OUT  5
#define PIN_BUZZER 6

Buzzer buzzer(PIN_BUZZER);
//notes
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
//buzzer setup
}

void loop() 
{
    colorData rgb;
    colorSensor.read();

    while (!colorSensor.available());

    colorSensor.getRGB(&rgb);
    print_rgb(rgb);
//play melody
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
