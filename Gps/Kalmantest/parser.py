#! /usr/bin/python
import pynmea2
import matplotlib.pyplot as plt
from rdp import rdp
import numpy as np
import mplleaflet
import pandas as pd
from pykalman import KalmanFilter



file1=open("/home/vivek/Documents/Gps/Kalman test/samplegps.log","r")
file2=open("/home/vivek/Documents/Gps/Kalman test/parsed_data.log","w",0)
counter=0
lat_d=[]
lon_d=[]
speed_kmph=[]
try:
	while 1:
		data = file1.readline()
		if (data.startswith("$GPRMC")):
			msg = pynmea2.parse(data)
			lat_d.append(round(34+((float(msg.lat) -3400)/60),6)) 
			lon_d.append(round(-118-((float(msg.lon) -11800)/60),6))
			speed_kmph.append(round(msg.spd_over_grnd*1.852,2))
			str_to_print = str(msg.timestamp)[0:8]\
			+ ', '\
			+ str( lat_d[counter] )\
			+ ', '\
			+ str( lon_d[counter] )\
			+ ', '\
			+ str(speed_kmph[counter])
			file2.write( str_to_print + "\n" )
			counter=counter+1
except KeyboardInterrupt:
	elev_d=np.zeros(504)

	measurements=np.column_stack((lon_d,lat_d,elev_d))
	# m2=rdp(m1,.00001) m1 - now - measurements
	F = np.array([[1, 0, 0, 1, 0, 0],
	[0, 1, 0, 0, 1, 0],
	[0, 0, 1, 0, 0, 1],
	[0, 0, 0, 1, 0, 0],
	[0, 0, 0, 0, 1, 0],
	[0, 0, 0, 0, 0, 1]])

	H = np.array([[1, 0, 0, 0, 0, 0],
	[0, 1, 0, 0, 0, 0],
	[0, 0, 1, 0, 0, 0]])

	R = np.diag([1e-4, 1e-4, 100])**2

	initial_state_mean = np.hstack([measurements[0, :], 3*[0.]])
	# works initial_state_covariance = np.diag([1e-3, 1e-3, 100, 1e-4, 1e-4, 1e-4])**2
	initial_state_covariance = np.diag([1e-4, 1e-4, 50, 1e-6, 1e-6, 1e-6])**2
	kf = KalmanFilter(transition_matrices=F, observation_matrices=H, observation_covariance=R,initial_state_mean=initial_state_mean,initial_state_covariance=initial_state_covariance,em_vars=['transition_covariance'])
	print("iterating..")
	kf = kf.em(measurements, n_iter=500)
	state_means, state_vars = kf.smooth(measurements)
	print("iteration complete.")
	plt.plot(state_means[:,0],state_means[:,1])
	plt.plot(state_means[:,0],state_means[:,1],'ro')

	mplleaflet.show()