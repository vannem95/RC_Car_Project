from mpl_toolkits.basemap import Basemap
#We need the actual pyplot library to make the plot
import matplotlib.pyplot as plt
#We are using CSV files, so to make our life easy why not use the csv libary..
import csv
            
#The first step is to read in the GPS coordiates form the file that contains the data.
#We know the latitude is in column 6, and the logitude is in column 8

#Create an empty array for each of the latitude and longitude
lats = []
longs = []

# Open the file and read the GPS data
with open("GPS_Clean.csv") as csvfile:
    #The 'csv' reader file lets us deal with CSV files easily
    reader = csv.reader(csvfile)
    #Since the reader is an 'iterator', we can skip the first line (the header)
    #By calling next on it. The for loop does the same thing, but we want to use the data
    next(reader, None)
    for row in reader:
        #Now we read the GPS data from column 6 and 8, noting that we need to turn the longitude
        #To a negative to indicate the Western hemisphere.
        x, y = float(row[6]), -1*float(row[8])
        #Add them to the array
        lats.append(x)
        longs.append(y)

#This creates the underlying map that we use. the first four arguments define the
#"Lower left" and "upper right" corners of the map in terms of lat/long. I just use the
#extreme values of the data to mark the map boundary. The "Mercator" type projection
#is used to allow the potential to use Open Street Map later (that's the system they use)
m = Basemap(llcrnrlon=min(longs),llcrnrlat=min(lats),urcrnrlon=max(longs),urcrnrlat=max(lats),lat_ts=20,
            resolution='c',projection='merc',lon_0=longs[0],lat_0=lats[0])

#The longitude and latitude lists we created are put through the map to create proper plot
#coordinates. NOTICE that the longitude and latitude are switched to produce proper x and y.
#Conventions can pose tricky problems like that.
x1, y1 = m(longs, lats)

#Now we fill the map background with white
m.drawmapboundary(fill_color='white')

#Now actually plot the GPS data. I've reduced the point size with 's' and played with the colours.
m.scatter(x1, y1, s=5, c='r', marker="o")

#Set the title
plt.title("GPS Bus Data")

#Display the plot
plt.show()