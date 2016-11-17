#! /usr/bin/python
import matplotlib.pyplot as plt
from rdp import rdp
import numpy as np
import mplleaflet
import pandas as pd
from pykalman import KalmanFilter
import utm

file1=open("/home/vivek/Documents/Gps/Kalmanexample/proj1log.txt","r")
file2=open("/home/vivek/Documents/Gps/Kalmanexample/parsed_data.txt","w",0)
lat_d=[]
lon_d=[]
speed_kmph=[]
latlon=[]
counter=0
while counter<1087:
		reader=file1.readline()
		latlon=utm.to_latlon(float(reader[14:25])+5*np.random.randn(),float(reader[31:42])+5*np.random.randn(),16,'T')
		lat_d.append(latlon[0])
		lon_d.append(latlon[1])
		# print(latlon)
		counter=counter+1
# plt.plot(lon_d,lat_d)
# plt.plot(lon_d,lat_d,'ro')
elev_d=np.zeros(1087)
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
initial_state_mean = np.hstack([measurements[1, :], 3*[0.]])

# works initial_state_covariance = np.diag([1e-3, 1e-3, 100, 1e-4, 1e-4, 1e-4])**2
initial_state_covariance = np.diag([1e-4, 1e-4, 50, 1e-6, 1e-6, 1e-6])**2
kf = KalmanFilter(
	transition_matrices=F, 
	observation_matrices=H, 
	observation_covariance=R,
	initial_state_mean=initial_state_mean,
	initial_state_covariance=initial_state_covariance,
	em_vars=['transition_covariance'])
print("iterating..")
kf = kf.em(measurements, n_iter=100)
state_means, state_vars = kf.smooth(measurements)
print("iteration complete.")
plt.plot(state_means[:,0],state_means[:,1])
plt.plot(state_means[:,0],state_means[:,1],'ro')
mplleaflet.show()