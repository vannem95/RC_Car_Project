from mpl_toolkits.basemap import Basemap

#We need the actual pyplot library to make the plot

import matplotlib.pyplot as plt

#we are using CSV files, so to make our life easy why not use the csv libary..

import csv

#The first step is to read in the GPS coordiates form the file that contains the data. We know the latitude is in column 6, and the logitude is in column 8

#Create an empty array for each of the latitude and longitude

lats = []
longs = []

#Open the file and read the GPS data

with open("gpsdata.csv") as csvfile:
 	
 	reader=csv.reader(csvfile)
	next(reader, None)
	for row in reader:
    	
    	x, y = float(row[4]), -1*float(row[6])
    	lats.append(x)
    	longs.append(y)
    	
m = Basemap(llcrnrlon=min(longs),llcrnrlat=min(lats),urcrnrlon=max(longs),urcrnrlat=max(lats),lat_ts=20,
            resolution='c',projection='merc',lon_0=longs[0],lat_0=lats[0])
x1, y1 = m(longs, lats)

m.drawmapboundary(fill_color='white')


m.scatter(x1, y1, s=5, c='r', marker="o")


plt.title("GPS Bus Data")


plt.show()