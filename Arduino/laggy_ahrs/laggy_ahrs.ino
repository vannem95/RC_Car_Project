// *************************************************************************************************ITG-3200_test
#include <MadgwickAHRS.h>
Madgwick filter;
unsigned long microsPerReading, microsPrevious;   

#include <Wire.h>
#include <ITG3200.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>


#include <Adafruit_ADXL345_U.h>

ITG3200 gyro = ITG3200();

float  x,y,z;
int ix, iy, iz;

/* Assign a unique ID to this sensor at the same time */
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);


/* Assign a unique ID to this sensor at the same time */
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);


void setup(void) {
#ifndef ESP8266
  while (!Serial); // for Leonardo/Micro/Zero
#endif
  Serial.begin(9600);
  
  filter.begin(5);
  
  Wire.begin();      
  delay(1000);
  gyro.init(ITG3200_ADDR_AD0_LOW); 
  gyro.zeroCalibrate(2500, 2);
  gyro.setFSRange(250);
  gyro.setSampleRateDiv(50);
  


  if(!mag.begin())
  {
    Serial.println("Ooops, no HMC5883 detected ... Check your wiring!");
    while(1);
  }
  if(!accel.begin())
  {
    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
    while(1);
  }
  accel.setRange(ADXL345_RANGE_2_G);
  accel.setDataRate(ADXL345_DATARATE_50_HZ);
  microsPerReading = 1000000 /50;
  microsPrevious = micros();
}

void loop(void) {
  float ax, ay, az;
  float gx, gy, gz;
  float roll, pitch, heading;
  unsigned long microsNow;
  if (gyro.isRawDataReady() && (microsNow - microsPrevious >= microsPerReading)) {
    // Reads calibrated values in deg/sec    
    gyro.readGyro(&x,&y,&z); 
    sensors_event_t event; 
    mag.getEvent(&event);

    float heading = atan2(event.magnetic.y, event.magnetic.x);
    float declinationAngle = 0.22;
    heading += declinationAngle;
    
    if(heading < 0)
      heading += 2*PI;
      
    if(heading > 2*PI)
      heading -= 2*PI;
     
    float headingDegrees = heading * 180/M_PI; 
    
    Serial.print("Heading (degrees): "); Serial.print(headingDegrees);
    Serial.print("\n");   
    ax = convertRawAcceleration(event.acceleration.x);
    ay = convertRawAcceleration(event.acceleration.y);
    az = convertRawAcceleration(event.acceleration.z);
    gx = convertRawGyro(x);
    gy = convertRawGyro(y);
    gz = convertRawGyro(z);
  
    // update the filter, which computes orientation
    filter.updateIMU(gx, gy, gz, ax, ay, az);
  
    // print the heading, pitch and roll
    roll = filter.getRoll();
    pitch = filter.getPitch();
    heading = filter.getYaw();
    Serial.print("--------------------------");
    Serial.print("\n");
    Serial.print("yaw/heading: ");
    Serial.print(heading);
    Serial.print("\t");
    Serial.print("pitch: ");
    Serial.print(pitch);
    Serial.print("\t");
    Serial.print("roll: ");
    Serial.println(roll);
    Serial.print("--------------------------");
    Serial.print("\n");
    microsPrevious = microsPrevious + microsPerReading;
  }
 else
 { 
    Serial.print("------raw data not ready--------");
    Serial.print("\n");
 }
}
float convertRawAcceleration(int aRaw) {
  // since we are using 2G range
  // -2g maps to a raw value of -32768
  // +2g maps to a raw value of 32767
  
  float a = (aRaw * 2.0) / 32768.0;
  return a;
}

float convertRawGyro(int gRaw) {
  // since we are using 250 degrees/seconds range
  // -250 maps to a raw value of -32768
  // +250 maps to a raw value of 32767
  
  float g = (gRaw * 250.0) / 32768.0;
  return g;
}
