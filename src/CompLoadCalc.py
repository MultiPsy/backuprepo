#!/usr/bin/env python
import pandas as pd
import numpy as np
import csv
import geopandas
import rasterio
from collections import Counter, defaultdict
import os
import time

os.chdir("/home/qweddww123/farsite/src")
os.system("make")

num_of_iter = 1

# csv file name
csv_name = './log'

# file type
csv_type_name = '.csv'
txt_type_name = '.txt'

# command name
cmd = "./TestFARSITE inputtext2" + txt_type_name

start = time.time()

for i in range(1, num_of_iter+1):
    returned_value = os.system(cmd + " > " + csv_name + str(i) + csv_type_name)

# .lcp file path
lcpfilepath = "/home/qweddww123/farsite/examples/Panther/test1177973/input/test1177973.lcp"

# open the file
raster = rasterio.open(lcpfilepath)

# computing time array
CompTimeArr = [[0 for i in range(raster.width)] for i in range(raster.height)]

list = []

# read csv file and put the values in arrary
for i in range(1, num_of_iter+1):
    list.append([])
    with open(csv_name + str(i) + csv_type_name, 'r') as file_name:
        csv_data = csv.reader(file_name)
        for line in csv_data:
            list[i-1].append(line)

for i in range(1, num_of_iter+1):
    # read csv file with pandas
    df = pd.read_csv(csv_name + str(i) + csv_type_name)

    # count the row and col
    row_cnt = len(df.axes[0]) + 1

    # sum the times
    for j in range(row_cnt):
        CompTimeArr[int(list[i-1][j][0])][int(list[i-1][j][1])] += float(list[i-1][j][2])
        # count the number of duplicated rows and cols (array name: list)
        
#print(row_cnt)

for i in range(raster.height):
    for j in range(raster.width):
        if CompTimeArr[i][j] != 0:
            continue
            #print("[" + str(i) + ", " + str(j) + ", " + str(round(((CompTimeArr[i][j])/num_of_iter),4)) + "]" + " (/ 10000000)")

print("time: ", time.time() - start)
#print(CompTimeArr)

arr = np.array(CompTimeArr)

df = pd.DataFrame(arr)
df.to_csv('/home/qweddww123/farsite/src/sample.csv', index=False)