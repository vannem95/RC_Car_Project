import serial
import pynmea2

ser = serial.Serial('/dev/ttyACM0',9600)

myfile=open('row.log','w')

while 1:
     data = ser.readline()
     if (data.startswith("$GPRMC")):
         msg = pynmea2.parse(data)
         str_to_print = str(msg.timestamp)\
                        + ', '\
                        + msg.lat\
                        + ', '\
                        + msg.lat_dir\
                        + ', '\
                        + msg.lon\
                        + ', '\
                        + msg.lon_dir\
                        + ', '\
                        + str(msg.spd_over_grnd) 
         print(str_to_print)
         # row = A+B+C+D+E+F+'/n'
         myfile.write( str_to_print + "\n" )
   