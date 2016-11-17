import sys
import serial 
from time import sleep
myfile=open('orientationloggertest.log','w')

try:
	serial_device=serial.Serial('/dev/ttyUSB0',57600, timeout=0.1)	
except serial.SerialException:
	print('Serial port error')

try:
	serial_arduino=serial.Serial('/dev/ttyACM5',57600)	
except serial.SerialException:
	print('Serial port error')
try:
	while (1):
		data = serial_arduino.readline()
		myfile.write( data )
		a=serial_device.read()
		serial_arduino.write(a)



except KeyboardInterrupt:
	print('Closing..')
	serial_arduino.close()
	serial_device.close()



