#! /usr/bin/python
import sys
import serial 
import rospy
from std_msgs.msg import UInt16
from time import sleep


class Communicator:
	start_character=''
	msg_types=''
	end_character=''
	def fail(self,error_message):
		print(error_message)
		exit(-1)
		return 
	def __init__(self):
		try:
			self.serial_device=serial.Serial('/dev/ttyUSB0',57600)	
		except serial.SerialException:
			self.fail('Serial port error')
		self.start_character='#'
		self.msg_types='c'
		self.end_character='\n'
	def make_string_from_value(self,value):
		value_as_string='%03d'%(value) 
		if len(value_as_string)>3:
			print('Error:Value out of range') 
			exit(-1)  

		return value_as_string
	def close_device(self):
		print('\nClosing serial device.\n')
		self.serial_device.close()

	def send(self,msg_type,value1,value2):
		msg=""
		if msg_type in self.msg_types:
			msg+=self.start_character
			msg+=msg_type
			msg+=self.make_string_from_value(value1)
			msg+=self.make_string_from_value(value2)
			msg+=self.end_character
		else:
			fail('Error:Wrong msg_type')
		print(msg)
		self.serial_device.write(msg)

	def rec(self):
		r=self.serial_device.readline()
		print(len(r))
		if len(r)<=1 | len(r)<=8:
			self.fail('msg too short')   

		if r[0] in self.start_character:
			if r[1] in self.msg_types:
				if r[8] in self.end_character:  
					r1=int(r[2:5])
					r2=int(r[5:8])
					
					print('throttle=%i' %(r1))
					print('steering=%i' %(r2))
				else:
					self.fail('Error:msg corrupted')
			else:
				self.fail('Error:Wrong msg_type')
		else:
			self.fail('Error:msg corrupted') 
		return (r1,r2)  

#test
# test=Communicator()
# while(1):
# 	try:
# 		sleep(0.5)
# 		
# 		print(parameters)


# def talker():
# 	global comm
# 	pub_throttle = rospy.Publisher('throttle', UInt16, queue_size=1)
# 	pub_steering = rospy.Publisher('steering', UInt16, queue_size=1)
# 	rospy.init_node('talker', anonymous=True)
# 	rate = rospy.Rate(1) # 1hz
# 	while not rospy.is_shutdown():
		
# 		parameters=comm.rec()
		
# 		# Making throttle msg
# 		throttle_msg=UInt16()
# 		throttle_msg.data=parameters[1]
# 		throttle_msg_as_string="sending throttle %i"%(throttle_msg.data)
# 		rospy.loginfo(throttle_msg_as_string)
# 		pub_throttle.publish(throttle_msg)

# 		# Making steering msg
# 		steering_msg=UInt16()
# 		steering_msg.data=parameters[2]
# 		steering_msg_as_string="sending steering %i"%(steering_msg.data)
# 		rospy.loginfo(steering_msg_as_string)
# 		pub_steering.publish(steering_msg)

# 		rate.sleep()

if __name__ == '__main__':
	#global comm=Communicator()
	try:
		talker()
	except rospy.ROSInterruptException:
		pass
	except KeyboardInterrupt:
		comm.close_device()
		sys.exit(0)