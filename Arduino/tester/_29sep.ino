char throttle_as_string[4];                     // defining constants and variables
char steering_as_string[4];

const int IDLE=1;
const int START=2;
const int CMD=3;
const int END=4;

int counter=0;
//int character_number=0;
char single_character; 

long int current_time;                               // timer
long int last_time=0;
const long int timeout=200;                        // timeout set to 200ms
  

const int THROTTLE_DEFAULT=90;
const int STEERING_DEFAULT=90;
int flag=0;                                           //debugger  

int last_throttle=THROTTLE_DEFAULT;
int last_steering=STEERING_DEFAULT;

int throttle=90;
int steering=90;

//char test_string[]="#c120090$";                    //tester

int elapsed_time_start;
int elapsed_time_cmd;
int elapsed_time_end;

int state = IDLE ;
//____________________________________________________________________________________//
void setup() {
  throttle=THROTTLE_DEFAULT;
  steering=STEERING_DEFAULT;
  Serial.begin(57600);                       // open the serial port at 9600 bps:
  state=IDLE;
  flag=0;
  
}
//_________ __________________________________________________________________________//
void loop()
{ 
  if (!(Serial.available()>0)){
    
  }
  else
  {
  single_character=Serial.read();
  current_time=millis();
  if (current_time-last_time>timeout){             // timer
    state=IDLE;
  }
  last_time=current_time;    
  switch(state)
  {                                                
    case IDLE:                                                                                                         
      if (single_character=='#')                   // idle state
      {
        state=START;  
      }
      throttle=last_throttle;
      steering=last_steering;

      counter=0;
      throttle_as_string[3]='\0';
      steering_as_string[3]='\0';

      break;
       
    case START:                                   // start state                                                                        
      if (single_character=='c')
      {
        state=CMD;  
      }

      break;
     
    case CMD:                                      //cmd - command state
      if (1==flag){    
        Serial.print("\t");                                //debug
        Serial.print("\t");           
        Serial.print("counter now:");
        Serial.print(counter);    
        Serial.print("\n");
      }        
      if (counter<=5)
      {
        if (single_character=='0'||single_character=='1'||single_character=='2'||single_character=='3'||single_character=='4'||single_character=='5'||single_character=='6'||single_character=='7'||single_character=='8'||single_character=='9')
        {  
          if (counter<=2)
          {      
            throttle_as_string[counter]=single_character;          
          }
          else
          {
            steering_as_string[counter-3]=single_character;
          }
          counter=counter+1;
        }
      }
      else{
                  if (single_character=='$')
          {
            state=END;
            throttle=char2num(throttle_as_string);    
            steering=char2num(steering_as_string);
            last_throttle=throttle;
            last_steering=steering;
            Serial.print("throttle:");
            Serial.print(throttle);
            Serial.print('\n'); 
            
            Serial.print("steering:");
            Serial.print(steering);
            Serial.print('\n');

          }
      }       
      break;                                       
      
    case END:                                        //end state
      state=IDLE;  

      
      if (1==flag){                                   //debug
        Serial.print('\n'); 
        
        Serial.print("throttle:");
        Serial.print(throttle);
        Serial.print('\n'); 
        
        Serial.print("steering:");
        Serial.print(steering);
        Serial.print('\n');
      }  

      break;        
  }
  if (1==flag){                                       //debug
    Serial.print("\t");
    Serial.print("Next State:");
    if (1==state){
      Serial.print("IDLE");
    }
    if (2==state){
      Serial.print("START");
    }    
    if (3==state){
      Serial.print("CMD");
    }
    if (4==state){
      Serial.print("END");
    }
    Serial.print("\n");
  }
  
  }
    
}
//_________________________________________________________________________//
int char2num(char x[4]){
  int result=100*((x[0])-'0') + 10*((x[1])-'0') + ((x[2])-'0');
  return result;
}

