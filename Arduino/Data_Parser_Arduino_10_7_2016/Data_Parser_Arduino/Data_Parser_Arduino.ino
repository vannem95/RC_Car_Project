#include <Servo.h> 

char throttle_as_string[4];                                  // defining constants and variables
char steering_as_string[4];

const int IDLE=1;
const int START=2;
const int CMD=3;
const int END=4;
const int TIMEOUT=0;

int counter=0;
//int character_number=0;
unsigned char single_character; 

int reciever_channel_2;                                       // receiever link pins  ch2 - throttle, ch4 - steering, ch5 - control witcher
int reciever_channel_4;
int reciever_channel_5;

long int current_time=0;                                      // timer
long int last_time_command_character=0;
const long int timeout_command_character=500;                // timeout set to 200ms

long int last_time_command=0;
const long int timeout_command=3000;                        // timeout set to 3000ms

const int THROTTLE_DEFAULT=90;
const int STEERING_DEFAULT=90;

const int THROTTLE_PIN=7;                                    // servo pins
const int STEERING_PIN=11;

Servo throttle_servo;
Servo steering_servo;

int led=13;
int flag=0;                                                 //debugger  
 
int last_throttle=THROTTLE_DEFAULT;
int last_steering=STEERING_DEFAULT;

int throttle=90;
int steering=90;

//char test_string[]="#c120090$";                          //tester

int elapsed_time_start;
int elapsed_time_cmd;
int elapsed_time_end;

int state = IDLE ;
//____________________________________________________________________________________//
void setup() {
  Serial.begin(57600);                                    // open the serial port at 57600 bps:
  state=IDLE;                                                                                              //**************************************************
  flag=0;                                                                                                  // FLAG IS SET TO ZERO. IF NOT THERE IS SOME ISSUE WITH SERIAL WRITE STUFF.
  throttle=THROTTLE_DEFAULT;
  steering=STEERING_DEFAULT;
  pinMode(led, OUTPUT);     
  current_time=millis();
  throttle_servo.attach(THROTTLE_PIN);
  steering_servo.attach(STEERING_PIN);
  write_commands(throttle,steering);
  pinMode(43, INPUT);                                     // Set our input pins as such (20-ch2-throttle 21-ch4-steering)
  pinMode(44, INPUT); 
  pinMode(45, INPUT);
}
//_________ __________________________________________________________________________//
void loop()
{
  reciever_channel_2 = pulseIn(43, HIGH, 25000);            // Read the pulse width of 
  reciever_channel_4 = pulseIn(44, HIGH, 25000);           // each channel
  reciever_channel_5 = pulseIn(45, HIGH, 25000);
//  Serial.print("Ch5:");
//  Serial.print(reciever_channel_5);
//  Serial.print("\n");
  
  reciever_channel_2 = constrain(reciever_channel_2, 850, 1850); 
  reciever_channel_2 = map(reciever_channel_2, 850, 1850, 80, 100); 

  
  reciever_channel_4 = constrain(reciever_channel_4, 850, 1850); 
  reciever_channel_4 = map(reciever_channel_4, 850, 1850, 45, 135); 


  if (reciever_channel_5<950 && reciever_channel_5>800){
    
//    Serial.print("Ch4, steering:");
//    Serial.print(reciever_channel_4);
//    Serial.print("\n");
//    Serial.print("Ch2, throttle:");
//    Serial.print(reciever_channel_2);
//    Serial.print("\n");
    write_commands(reciever_channel_2,reciever_channel_4);
  }
  else{
    current_time=millis();
    if (current_time-last_time_command>timeout_command){     // timer - command
      throttle=THROTTLE_DEFAULT;
      steering=STEERING_DEFAULT;
      digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
    }
    else{
      digitalWrite(led, LOW);
    }
    write_commands(throttle,steering);
    
    if (!(Serial.available()>0)){
      
    }
    else
    {
      single_character=Serial.read();
      
      //  digitalWrite(led, HIGH);                            // turn the LED on (HIGH is the voltage level)
      //  delay(200);                                            // wait for a second
      //  digitalWrite(led, LOW);
      //  delay(200);
      
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
          if (single_character=='#')                                  // idle state
          {
            state=START;  
          }
          throttle=last_throttle;
          steering=last_steering;
          counter=0;
          throttle_as_string[3]='\0';
          steering_as_string[3]='\0';
          
          break;
           
        case START:                                                 // start state                                                                        
          if (single_character=='c')
          {
            state=CMD;  
          }
          
          break;
          
        case CMD:                                                 //cmd - command state
          if (1==flag){    
  //          Serial.print("\t");                                   //debug
  //          Serial.print("\t");           
  //          Serial.print("counter now:");
  //          Serial.print(counter);    
  //          Serial.print("\n");
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
  //            Serial.print("throttle:");
  //            Serial.print(throttle);
  //            Serial.print('\n');
  //            Serial.print("steering:");
  //            Serial.print(steering);
  ////            Serial.print('\n');
            }
          }
          break;                                       
           
      }
  //    if (1==flag)
  //    {
  //      Serial.print("\t");
  //      Serial.print("Next State:");
  //      if (0==state){
  //        Serial.print("TIMEOUT");
  //      }
  //      if (1==state){
  //        Serial.print("IDLE");
  //      }
  //      if (2==state){
  //        Serial.print("START");
  //      }    
  //      if (3==state){
  //        Serial.print("CMD");
  //      }
  //      if (4==state){
  //        Serial.print("END");
  //      }
  //      Serial.print("\n");
  //    }
  
    }
  }
}
//_________________________________________________________________________//
int char2num(char x[4]){
  int result=100*((x[0])-'0') + 10*((x[1])-'0') + ((x[2])-'0');
  return result;
}
void write_commands(int throttle_command,int steering_command){
  throttle_command = constrain(throttle_command, 80, 96); 
  steering_command = constrain(steering_command, 45, 135); 
  throttle_servo.write(throttle_command);
  steering_servo.write(steering_command);
}

