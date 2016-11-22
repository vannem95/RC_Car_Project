import sys
import serial
from time import sleep
orientation=open('/home/pi/orientation.log','w')
gps=open('/home/pi/rawgps.log','w')
DEBUG=0
try:
	serial_device=serial.Serial('/dev/ttyUSB0',57600, timeout=0.1)
except serial.SerialException:
	print('Serial port(Telemetry) error')

try:
	serial_arduino=serial.Serial('/dev/ttyACM0',57600,timeout=0.01)
except serial.SerialException:
	print('Serial port(Arduino) error')
try:
	serial_gps=serial.Serial('/dev/ttyAMA0',9600,timeout=0.01)
except serial.SerialException:
	print('Serial port(Gps) error')
try:
	while (1):
		imudata = serial_arduino.readline()
		orientation.write(imudata)
		gpsdata = serial_gps.readline()
		gps.write(gpsdata)
		telemetrydata=serial_device.read()
                if 1==DEBUG :
                   print(telemetrydata)
		serial_arduino.write(telemetrydata)
except KeyboardInterrupt:
	print('Closing..')
	serial_arduino.close()
	serial_device.close()
