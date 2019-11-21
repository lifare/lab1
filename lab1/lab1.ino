#include <Arduino.h>
#include <MD_TCS230.h>
#include "Wire.h"

#define  S0_OUT  2
#define  S1_OUT  3
#define  S2_OUT  4
#define  S3_OUT  5

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

MD_TCS230 colorSensor(S2_OUT, S3_OUT, S0_OUT, S1_OUT);

const int address = 10;
int currentColor = none;

void setup()
{
    Serial.begin(115200);
    Serial.println("Started!");

  sensorData whiteCalibration;
  whiteCalibration.value[TCS230_RGB_R] = 80200;
  whiteCalibration.value[TCS230_RGB_G] = 75240;
  whiteCalibration.value[TCS230_RGB_B] = 102750;
  
  sensorData blackCalibration;
  blackCalibration.value[TCS230_RGB_R] = 11410;
  blackCalibration.value[TCS230_RGB_G] = 10400;
  blackCalibration.value[TCS230_RGB_B] = 14030;




    colorSensor.begin();
    colorSensor.setDarkCal(&blackCalibration);
    colorSensor.setWhiteCal(&whiteCalibration);

    Wire.begin(address);
    Wire.onRequest(handleRequest);
}

void loop() 
{
    colorData rgb;
    colorSensor.read();
    while (!colorSensor.available());
    colorSensor.getRGB(&rgb);
    print_rgb(rgb);
    currentColor = getColor(rgb);
    Serial.print("\n");
    Serial.print(currentColor);
    Serial.print("\n");
}

int getColor(colorData rgb){
  int r = rgb.value[TCS230_RGB_R];
  int g = rgb.value[TCS230_RGB_G];
  int b = rgb.value[TCS230_RGB_B];
  int dif = 10;
  int reds[] = {143, 17, 41};
  int yellows[] = {255, 189, 73};
  int oranges[] = {209, 49, 26};
  int purples[] = {32, 31, 72};
  int greens[] ={22, 60, 27};
  int blues[] ={35, 116, 171};
  if (r < dif && g < dif && b < dif)
    return black;
  if (255 - r < dif && 255 - g < dif && 255 - b < dif)
    return white;
  if (abs(purples[0]- r) < dif && abs(purples[1] - g) < dif && abs(purples[2] - b) < dif)
    return purple;
  if (abs(yellows[0]- r) < dif && abs(yellows[1] - g) < dif && abs(yellows[2] - b) < dif)
    return yellow;
  if (abs(oranges[0]- r) < dif && abs(oranges[1] - g) < dif && abs(oranges[2] - b) < dif)
    return orange;
  if (abs(reds[0]- r) < dif && abs(reds[1] - g) < dif && abs(reds[2] - b) < dif)
    return red;
  if ((abs(greens[0]- r) < dif && abs(greens[1] - g) < dif && abs(greens[2] - b) < dif))
    return green;
  if ((abs(blues[0]- r) < dif && abs(blues[1] - g) < dif && abs(blues[2] - b) < dif))
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

void handleRequest() {
  Wire.write(currentColor);
}
