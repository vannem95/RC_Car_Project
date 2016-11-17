/*
 * rosserial Servo Control Example
 *
 * This sketch demonstrates the control of hobby R/C servos
 * using ROS and the arduiono
 * 
 * For the full tutorial write up, visit
 * www.ros.org/wiki/rosserial_arduino_demos
 *
 * For more information on the Arduino Servo Library
 * Checkout :
 * http://www.arduino.cc/en/Reference/Servo
 */
//#define MIN_THROTTLE_FRONT (96)
//#define MAX_THROTTLE_FRONT (120)
//
//#define MIN_THROTTLE_BACK (84)
//#define MAX_THROTTLE_BACK (60)
//
//#define MIN_STEER (45)
//#define MAX_STEER (135)


#define MIN_THROTTLE_FRONT (0)
#define MAX_THROTTLE_FRONT (180)

#define MIN_THROTTLE_BACK (180)
#define MAX_THROTTLE_BACK (0)

#define MIN_STEER (0)
#define MAX_STEER (180)

#define NEUTRAL (90)

#if defined(ARDUINO) && ARDUINO >= 100
   #include "Arduino.h"
 #else
   #include <WProgram.h>
 #endif
 
   #include <Servo.h> 
   #include <ros.h>
   #include <std_msgs/UInt16.h>
   ros::NodeHandle  nh;

Servo steering_servo;
Servo throttle_servo;

void steering_servo_cb( const std_msgs::UInt16& cmd_msg){
  int value=constrain(cmd_msg.data,MIN_STEER,MAX_STEER);
  steering_servo.write(value);                                                              //within a loop, a local variable can be    
  digitalWrite(13, HIGH-digitalRead(13));                                                   //a common name
}
void throttle_servo_cb( const std_msgs::UInt16& cmd_msg){
  int value;
//  if (cmd_msg.data<NEUTRAL){
//    value=constrain(cmd_msg.data,MAX_THROTTLE_BACK,MIN_THROTTLE_BACK);
//    }
//  else{
//    value=constrain(cmd_msg.data,MIN_THROTTLE_FRONT,MAX_THROTTLE_FRONT);
//    }   
  value=cmd_msg.data;
  throttle_servo.write(value); //set servo angle, should be from 0-180  
  digitalWrite(13, HIGH-digitalRead(13));  //toggle led  
}
ros::Subscriber<std_msgs::UInt16> steering_sub("steering", steering_servo_cb);              // name of the subscriber callback has
ros::Subscriber<std_msgs::UInt16> throttle_sub("throttle", throttle_servo_cb);              // to match the one defined above

void setup(){
  pinMode(13, OUTPUT);
 // ALWAYS ONLY ONE SETUP                              
  nh.initNode();
  
  nh.subscribe(steering_sub);                                                               // name of the subscriber callback has
  nh.subscribe(throttle_sub);                                                               // to match the one defined above
  steering_servo.attach(11); //attach it to pin 7
  throttle_servo.attach(7); //attach it to pin 6
  throttle_servo.write(NEUTRAL);
  steering_servo.write(NEUTRAL);
}   

void loop(){
  nh.spinOnce();
  delay(1);
}
