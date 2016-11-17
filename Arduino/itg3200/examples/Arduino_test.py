import sys
import serial 
from time import sleep
global sleep_time
command_1="#c090120$"
command_2="#c090060$"
command_3="#c090090$"
command_4="#c096090$"
command_5="#c090090$"
global serial_device

sleep_time=3
counter=1

def write_and_flush(command):
	global sleep_time
	global serial_device
	# serial_device.flushOutput()
	serial_device.write(command)
	print('sent')
	sleep(sleep_time)


try:
	serial_device=serial.Serial('/dev/ttyUSB0',57600)
	print("Now starting..")
	sleep(2)
except serial.SerialException:
	print('Serial port error')

try:
	while (1):
		print('%d.'%counter)
		print(serial_device.is_open)
		write_and_flush(command_1)
		write_and_flush(command_2)
		write_and_flush(command_3)
		write_and_flush(command_4)
		write_and_flush(command_5)
		counter=counter+1
except KeyboardInterrupt:
	print('Closing..')
	serial_device.close()





