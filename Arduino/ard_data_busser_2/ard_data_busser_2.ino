 #include <Servo.h> 
//------------------------------------------------------

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include <Adafruit_Simple_AHRS.h>
#include <Adafruit_ADXL345_U.h>
#include <ITG3200.h>

Adafruit_ADXL345_Unified accel(11111);
Adafruit_HMC5883_Unified mag(22222);
ITG3200 gyro = ITG3200();
float xyz[3], temperature;

Adafruit_Simple_AHRS          ahrs(&accel, &mag);
int sensorcounter=0;
const int SENSOR_FREQUENCY=20;

//------------------------------------------------------
char throttle_as_string[4];                                                                              // defining constants and variables
char steering_as_string[4];

const int IDLE=1;
const int START=2;
const int CMD=3;
const int END=4;
const int TIMEOUT=0;

int counter=0;
//int character_number=0;
unsigned char single_character; 
 
int reciever_channel_2;                                                                        // receiever link pins  ch2 - throttle, ch4 - steering, ch5 - control witcher
int reciever_channel_4;
int reciever_channel_5;

long int current_time=0;                                                                                                 // timer
long int last_time_command_character=0;
const long int timeout_command_character=500;                                                                   // character timeout set to 500ms
   
long int last_time_command=0;
const long int timeout_command=3000;                                                                             // command timeout set to 3000ms

const int THROTTLE_DEFAULT=90;
const int STEERING_DEFAULT=90;
 
const int THROTTLE_PIN=7;                                                                                             // servo pins
const int STEERING_PIN=11;

Servo throttle_servo;
Servo steering_servo;

int led=13;
int flag=0;                                                                                                              //debugger  
 
int last_throttle=THROTTLE_DEFAULT;
int last_steering=STEERING_DEFAULT;

int throttle=90;
int steering=90;

//char test_string[]="#c120090$";                                                                                          //tester
 
int elapsed_time_start;
int elapsed_time_cmd;
int elapsed_time_end;

int state = IDLE ;

unsigned long timer[5];
byte last_channel[4];
int input[4];


void setup() {

  PCICR |= (1 << PCIE0);                                                                                              // interrupt initiation
  PCMSK0 |= (1 << PCINT1);
                                                                                                                // open the serial port at 57600 bps:

  Serial.begin(57600); 
  
  pinMode(8, INPUT); 
  pinMode(40, INPUT);
  
  state=IDLE;                                                                                              //**************************************************
  flag=0;                                                                                                  // FLAG IS SET TO ZERO. IF NOT THERE IS SOME ISSUE WITH SERIAL WRITE STUFF.
  throttle=THROTTLE_DEFAULT;
  steering=STEERING_DEFAULT;
  current_time=millis();
  throttle_servo.attach(THROTTLE_PIN);
  steering_servo.attach(STEERING_PIN);
  write_commands(throttle,steering);
//------------------------------------------------------
  accel.begin();
  mag.begin();
//------------------------------------------------------

  Wire.begin();                                                                                         // if experiencing gyro problems/crashes while reading XYZ values
                                                                                                          // please read class constructor comments for further info.
  delay(1000);
  gyro.reset();
                                                                                                      // Use ITG3200_ADDR_AD0_HIGH or ITG3200_ADDR_AD0_LOW as the ITG3200 address 
                                                                                             // depending on how AD0 is connected on your breakout board, check its schematics for details
  gyro.init(ITG3200_ADDR_AD0_LOW);
	
  Serial.print("zero Calibrating...");
  gyro.zeroCalibrate(2500,2);
  Serial.println("  done."); 
}

void loop()
{
//------------------------------------------------------
  sensors_event_t event; 
  accel.getEvent(&event);
  
  sensorcounter=sensorcounter+1;                                                                         //***************************** LOOP BEGINS*****************************
  sensors_vec_t   orientation;
  if (ahrs.getOrientation(&orientation) && 0==sensorcounter%SENSOR_FREQUENCY && gyro.isRawDataReady())                              // Initialize sensors
  {
    gyro.readGyro(xyz);
    float wx=float((1/180)*PI*(xyz[0])); 
    float wy=float((1/180)*PI*(xyz[1])); 
    float wz=float((1/180)*PI*(xyz[2]));    
   

    float roll=float((1/180)*PI*(orientation.roll));    
    float pitch=float((1/180)*PI*(orientation.pitch));                                                                                     // orientation, linear acceleration, angular velocity messages being sent to RPI-Python
    float heading=float((1/180)*PI*(orientation.heading));
    char str[50];
    sprintf(str, "r%01d.%02dp%01d.%02dh%01d.%02da%02d.%02da%02d.%02da%02d.%02dw%02d.%02dw%02d.%02dw%02d.%02d\n",  (int)((orientation.roll)*PI/180),(int)(signcheck(orientation.roll)*100)%100,(int)((orientation.pitch)*PI/180),(int)(signcheck(((orientation.pitch)*PI/180))*100)%100,(int)((orientation.heading)*PI/180),(int)(signcheck(((orientation.heading)*PI/180))*100)%100,  (int)(event.acceleration.x),(int)(signcheck(event.acceleration.x)*100)%100,(int)(event.acceleration.y),(int)(signcheck(event.acceleration.y)*100)%100,(int)(event.acceleration.z),(int)(signcheck(event.acceleration.z)*100)%100,  (int)((xyz[0])*PI/180),(int)(signcheck(((xyz[0])*PI/180))*100)%100,(int)((xyz[1])*PI/180),(int)(signcheck(((xyz[1])*PI/180))*100)%100,(int)((xyz[2])*PI/180),(int)(signcheck(((xyz[2])*PI/180))*100)%100);
    Serial.write(str);
  }
//------------------------------------------------------

  if (input[1]>900 & input[1]<1050)                                                                         // checking interrupt value of the switch(transmitter channel#5)
  {
    reciever_channel_2 = pulseIn(8, HIGH, 25000);
    reciever_channel_4 = pulseIn(40, HIGH, 25000);                                                // if the transmitter switch is turned on, channels 2(throttle) and 4(steering) are read, mapped to servo readable values and written to the servos                   
    
    reciever_channel_2 = constrain(reciever_channel_2, 850, 1850); 
    reciever_channel_2 = map(reciever_channel_2, 850, 1850, 80, 100); 
    
    reciever_channel_4 = constrain(reciever_channel_4, 850, 1850); 
    reciever_channel_4 = map(reciever_channel_4, 850, 1850, 45, 135);
    
    write_commands(reciever_channel_2,reciever_channel_4);
  }
  else
  {


      current_time=millis();
      if (current_time-last_time_command>timeout_command){                                                       // timer - command
        throttle=THROTTLE_DEFAULT; 
        steering=STEERING_DEFAULT;
      }
      else{
      }
      write_commands(throttle,steering);
      if (!(Serial.available()>0)){
      }
      else
      {
        single_character=Serial.read();
        current_time=millis();
        if (current_time-last_time_command_character>timeout_command_character && state!=IDLE){             // timer - command character
          state=IDLE;
          last_throttle=THROTTLE_DEFAULT;
          last_steering=STEERING_DEFAULT;
        }
        last_time_command_character=current_time;    
        switch(state)
        {     
          case IDLE:                                                                                                         
            if (single_character=='#')                                                                           // idle state
            { 
              state=START;  
            }
            throttle=last_throttle;
            steering=last_steering;
            counter=0;
            throttle_as_string[3]='\0';
            steering_as_string[3]='\0';
            break;
          case START:                                                                                             // start state                                                                        
            if (single_character=='c')
            {
              state=CMD;  
            }
            break;
          case CMD:                                                                                              //cmd - command state
            if (1==flag){    
  
            }        
            if (counter<=5)
            {
              if (single_character=='0'||single_character=='1'||single_character=='2'||single_character=='3'||single_character=='4'||single_character=='5'||single_character=='6'||single_character=='7'||single_character=='8'||single_character=='9')
              {  
                if (counter<=2)
                {      
                  throttle_as_string[counter]=single_character;          
                }
                else{
                  steering_as_string[counter-3]=single_character;
                }
                counter=counter+1;
              }
            }
            else{
              if (single_character=='$'){
                state=IDLE;
                throttle=char2num(throttle_as_string);
                steering=char2num(steering_as_string);
                last_throttle=throttle;
                last_steering=steering;
                last_time_command=current_time;    
              }
            }
            break;                                       
        }
      }
  }
}

int char2num(char x[4]){                                                                                         // character to number converter
  int result=100*((x[0])-'0') + 10*((x[1])-'0') + ((x[2])-'0');
  return result;
}
void write_commands(int throttle_command,int steering_command){                                                       // command writer

  throttle_servo.write(throttle_command);
  steering_servo.write(steering_command);
}


ISR(PCINT0_vect) {                                                                                                 // Pulse width measurement using interrupt
  timer[0] = micros();
  // channel 2 ---------------
  if(last_channel[1] == 0 && PINB & B00000010 ) {
    last_channel[1] = 1;
    timer[2] = timer[0];
  }
  else if(last_channel[1] == 1 && !(PINB & B00000010) ) {
    last_channel[1] = 0;
    input[1] = timer[0] - timer[2];
  }
}
float signcheck(float x){
  float result;
  if (0>x){
    result=-x;
  }
  else{
    result=x;
  }
  return result;
}

