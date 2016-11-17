#!/usr/bin/python

import numpy as np
import matplotlib.pyplot as plt

with open("/home/vivek/Documents/Gps/Matplotlibstuff/test_new.txt") as f:
    data = f.readlines()

x = [row.split(', ')[1] for row in data]
y = [row.split(', ')[3] for row in data]

lat=[((float(a)-3400)/60)+34 for a in x]
lon=[((float(a)-11800)/60)+118 for a in y]


fig = plt.figure()

ax1 = fig.add_subplot(111)

ax1.set_title("lat lon")    
ax1.set_xlabel('lat')
ax1.set_ylabel('lon')

ax1.plot(lat,lon, c='r', label='the data')

leg = ax1.legend()

plt.show()