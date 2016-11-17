// *************************************************************************************************ITG-3200_test
#include <MadgwickAHRS.h>
Madgwick filter;


#include <Wire.h>
#include <ITG3200.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

#include <Adafruit_BMP085.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

ITG3200 gyro = ITG3200();
float  x,y,z;
int ix, iy, iz;

/* Assign a unique ID to this sensor at the same time */
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

Adafruit_BMP085 bmp;

/* Assign a unique ID to this sensor at the same time */
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);


void setup(void) {
#ifndef ESP8266
  while (!Serial); // for Leonardo/Micro/Zero
#endif
  Serial.begin(9600);
  
  filter.begin();
  
  Wire.begin();      
  delay(1000);
  gyro.init(ITG3200_ADDR_AD0_LOW); 
  gyro.zeroCalibrate(2500, 2);
  if (!bmp.begin()) {
	Serial.println("Could not find a valid BMP085 sensor, check wiring!");
	while (1) {}
  }

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
  displaySetRange(ADXL345_RANGE_2_G);

  
}

void loop(void) {
  int aix, aiy, aiz;
  int gix, giy, giz;
  float ax, ay, az;
  float gx, gy, gz;
  float roll, pitch, heading;
  while (gyro.isRawDataReady()) {
    // Reads calibrated values in deg/sec    
    gyro.readGyro(&x,&y,&z); 
    Serial.print("\n");
    Serial.print("-------------------------------------------------------------------------------------"); 
    Serial.print("\n"); 
    Serial.print("GYRO VALUES:"); 
    Serial.print("\n"); 

    Serial.print("X3:"); 
    Serial.print(x); 
    Serial.print("  Y:"); 
    Serial.print(y); 
    Serial.print("  Z:"); 
    Serial.print(z);
    Serial.print("\n");
    
    sensors_event_t event; 
    mag.getEvent(&event);
    Serial.print("\n"); 
    Serial.print("-  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -"); 
    Serial.print("\n"); 
    Serial.print("MAGNETOMETER VALUES:");
    Serial.print("\n");
    /* Display the results (magnetic vector values are in micro-Tesla (uT)) */
    Serial.print("X: "); Serial.print(event.magnetic.x); Serial.print("  ");
    Serial.print("Y: "); Serial.print(event.magnetic.y); Serial.print("  ");
    Serial.print("Z: "); Serial.print(event.magnetic.z); Serial.print("  ");Serial.print("uT");
    Serial.print("\n");
  
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
    
    
    Serial.print("Altitude = ");
    Serial.print(bmp.readAltitude());
    Serial.print(" meters");
    Serial.print("\n");
  
    Serial.print("Real altitude = ");
    Serial.print(bmp.readAltitude(101500));
    Serial.print(" meters");
    Serial.print("\n");

    Serial.print(event.acceleration.x); 
    Serial.print(event.acceleration.y); 
    Serial.print(event.acceleration.z); Serial.println("m/s^2 ");
    
    
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
    Serial.print("Orientation: ");
    Serial.print(heading);
    Serial.print(" ");
    Serial.print(pitch);
    Serial.print(" ");
    Serial.println(roll);
    delay(2000);
  } 
}











void loop() {



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
