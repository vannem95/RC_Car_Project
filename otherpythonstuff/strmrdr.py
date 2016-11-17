import pynmea2
streamreader = pynmea2.NMEAStreamReader("/dev/ttyACM0")
while 1:
    	for msg in streamreader.next():
        	print msg