import os
from random import *
import datetime as dt

RAWS_num = 80
shp_path = '/home/qweddww123/farsite/TrainDataGen/WildfireShapefiles'

def calander(months):
    if months == 2:
        return False
    if months == 4:
        return False
    if months == 6:
        return False
    if months == 9:
        return False
    if months == 11:
        return False
    else:
        return True

def calc_24(hours, RAWS_num):
    hours += RAWS_num * 100
    if hours % 100 == 0:
        return hours % 2400 - 41
    else:
        return hours % 2400 - 1

for years in sorted(os.listdir(shp_path), key=int):
    for wildfire_name in sorted(os.listdir(shp_path+'/'+years), key=str):
        if(wildfire_name[0] == '.'):
            continue
        elif(wildfire_name.split('_')[0] == 'ak' or wildfire_name.split('_')[0] == 'AK'):
            continue
        elif(wildfire_name.split('_')[0] == 'hi'):
            continue
        else:
            print()
            year = ''
            months = ''
            days = ''
            hours = ''
            #read from shapefiles
            wildfire_time = sorted(os.listdir(shp_path+'/'+years+'/'+wildfire_name))[0]
            cnt = 0
            for i in wildfire_time:
                if i == '_':
                    cnt += 1
                    continue
                if cnt < 4:
                    year += i
                if cnt >= 4 and cnt < 6:
                    months += i
                if cnt >= 6 and cnt < 8:
                    days += i
                if cnt >= 9:
                    hours += i
                cnt += 1
            #save str wildfire_time to int
            try:
                int_year = int(year)
                int_months = int(months)
                int_days = int(days)
                int_hours = int(hours)
            except ValueError:
                break
                
            #print(wildfire_name)
            #print(wildfire_time)
            #print(int_year, int_months, int_days, int_hours)

            burn_periods = 4
            int_end_hours = calc_24(int_hours, RAWS_num-1)
            end_hours = str(int_end_hours)
            #print("end_hours =", end_hours)
            start_hours = str(int_hours)
            #print("start_time =", start_hours)

            burn_periods_str = ""
            int_burn_days = int_days
            int_burn_months = int_months
            for i in range(burn_periods):
                #month day start_hour end_hour
                if int_burn_days > 30 and calander(int_burn_months) != True:    #did not consider Feburary
                    int_burn_months += 1
                    int_burn_days = 1
                if int_burn_months > 12:
                    int_burn_months = 1

                str1 = str(int_burn_months)+' '+str(int_burn_days)+' '+start_hours+' '+end_hours+'\n'
                burn_periods_str += str1

                int_burn_days += 1

            #print(burn_periods_str)

            RAWS_str = ""
            for i in range(RAWS_num):
                if int_hours >= 2400:
                    int_days += 1
                    int_hours %= 2400
                if int_days > 30 and calander(int_months) != True:    #did not consider Feburary
                    int_months += 1
                    int_days = 1
                if int_months > 12:
                    int_year += 1
                    int_months = 1
                    
                #year month day hour temperature humidity percipitation wind_speed wind_direction cloud_cover
                str2 = str(int_year)+' '+str(int_months)+' '+str(int_days)+' '+str(int_hours)+' '+str(randrange(50, 86))+' '+str(randrange(10, 81))+' '+str(0.00)+' '+str(randrange(2, 15))+' '+str(randrange(0, 360))+' '+str(randrange(0, 66))+'\n'
                RAWS_str += str2
                int_hours += 100
                
            #print(RAWS_str)
            if (int_end_hours+1 ) % 60 == 0:
                int_end_hours = int_end_hours + 1 + 40

            input_string = "FARSITE INPUTS FILE VERSION 1.0\n\
FARSITE_START_TIME: "+months+' '+days+' '+start_hours+"\n\
FARSITE_END_TIME: "+str(int_burn_months)+' '+str(int_burn_days-1)+' '+str(int_end_hours)+"\n\
FARSITE_TIMESTEP: 60\n\
FARSITE_DISTANCE_RES: 30.0\n\
FARSITE_PERIMETER_RES: 60.0\n\
FARSITE_MIN_IGNITION_VERTEX_DISTANCE: 15.0\n\
FARSITE_SPOT_GRID_RESOLUTION: 15.0\n\
FARSITE_SPOT_PROBABILITY: 0.05\n\
FARSITE_SPOT_IGNITION_DELAY: 0\n\
FARSITE_MINIMUM_SPOT_DISTANCE: 30\n\
FARSITE_ACCELERATION_ON: 1\n\
FARSITE_FILL_BARRIERS: 1\n\
SPOTTING_SEED: 253114\n\
\n\
FARSITE_BURN_PERIODS: "+str(burn_periods)+"\n\
"+burn_periods_str+"\n\
\n\
FUEL_MOISTURES_DATA: 1\n\
0 3 4 6 70 100\n\
RAWS_ELEVATION: 4159\n\
RAWS_UNITS: English\n\
\n\
RAWS: "+str(RAWS_num)+"\n\
"+RAWS_str+"\n\
\n\
FOLIAR_MOISTURE_CONTENT: 100\n\
CROWN_FIRE_METHOD: Finney\n\
NUMBER_PROCESSORS: 1\n\
\n\
\n\
FLAMELENGTH:\n\
SPREADRATE:\n\
INTENSITY:\n\
CROWNSTATE:\n\
"

            #print(input_string)
            inputfilepath = shp_path+'/'+years+'/'+wildfire_name+'/'+wildfire_time+'/'+wildfire_time+".input"

            f = open(inputfilepath, "w")
            f.write(input_string)
            f.close()
