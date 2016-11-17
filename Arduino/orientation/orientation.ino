#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include <Adafruit_Simple_AHRS.h>
#include <Adafruit_ADXL345_U.h>

Adafruit_ADXL345_Unified accel(11111);
Adafruit_HMC5883_Unified mag(22222);

Adafruit_Simple_AHRS          ahrs(&accel, &mag);



void setup()
{
  Serial.begin(57600);
  Serial.println(F("Adafruit 10 DOF Board AHRS Example")); Serial.println("");
  accel.begin();
  mag.begin();
}

void loop(void)
{
  sensors_vec_t   orientation;

  if (ahrs.getOrientation(&orientation))
  {
    Serial.print(F("Orientation: "));
    Serial.print(orientation.roll*3.14/180);
    Serial.print(F(" "));
    Serial.print(orientation.pitch*3.14/180);
    Serial.print(F(" "));
    Serial.print(orientation.heading*3.14/180);
    Serial.println(F(""));
  }
  delay(100);
}


