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

#define MIN_THROTTLE_FRONT (96)
#define MAX_THROTTLE_FRONT (120)

#define MIN_THROTTLE_BACK (84)
#define MAX_THROTTLE_BACK (60)

#if defined(ARDUINO) && ARDUINO >= 100
   #include "Arduino.h"
 #else
   #include <WProgram.h>
 #endif
 
   #include <Servo.h> 
   #include <ros.h>
   #include <std_msgs/UInt16.h>
   ros::NodeHandle  nh;
 Servo servo;
   
  void servo_cb( const std_msgs::UInt16& cmd_msg){
    if cmd_msg.data<90{
      int value=constrain(cmd_msg.data,MIN_THROTTLE_BACK,MAX_THROTTLE_BACK);
      }
    else{
      int value=constrain(cmd_msg.data,MIN_THROTTLE_FRONT,MAX_THROTTLE_FRONT);
      }     
    servo.write(value); //set servo angle, should be from 0-180  
    digitalWrite(13, HIGH-digitalRead(13));  //toggle led  
  }
  ros::Subscriber<std_msgs::UInt16> sub("servo", servo_cb);
  void setup(){
    pinMode(13, OUTPUT);
    nh.initNode();
    nh.subscribe(sub);
    servo.attach(7); //attach it to pin 9
  }
  void loop(){
    nh.spinOnce();
    delay(1);
  }
