#!/usr/bin/env python
'''
generate field tables from IGRF12
'''

import pyigrf12 as igrf
import numpy as np
import datetime
<<<<<<< HEAD
import os.path
import sys
from math import sqrt, pi, atan2, asin

if not os.path.isfile("AP_Declination.h"):
    print("Please run this tool from the AP_Declination directory")
    sys.exit(1)


def field_to_angles(x, y, z):
    intensity = sqrt(x**2+y**2+z**2)
    r2d = 180.0 / pi
    declination = r2d * atan2(y, x)
    inclination = r2d * asin(z / intensity)
    return [intensity, inclination, declination]


isv = 0
date = datetime.datetime.now()
itype = 1
alt = 0.0

SAMPLING_RES = 10
SAMPLING_MIN_LAT = -90
SAMPLING_MAX_LAT = 90
SAMPLING_MIN_LON = -180
SAMPLING_MAX_LON = 180

NUM_LAT = 1 + (SAMPLING_MAX_LAT - SAMPLING_MIN_LAT) / SAMPLING_RES
NUM_LON = 1 + (SAMPLING_MAX_LON - SAMPLING_MIN_LON) / SAMPLING_RES

intensity_table = np.empty((NUM_LAT, NUM_LON))
inclination_table = np.empty((NUM_LAT, NUM_LON))
declination_table = np.empty((NUM_LAT, NUM_LON))

for i in range(NUM_LAT):
    for j in range(NUM_LON):
        lat = SAMPLING_MIN_LAT + i*SAMPLING_RES
        lon = SAMPLING_MIN_LON + j*SAMPLING_RES
        x, y, z, f, d = igrf.gridigrf12(date, isv, itype, alt, lat, lon)
        intensity, I, D = field_to_angles(x, y, z)
        intensity_table[i][j] = intensity * 0.00001
        inclination_table[i][j] = I
        declination_table[i][j] = D

tfile = open("tables.cpp", 'w')
tfile.write('''// this is an auto-generated file from the IGRF tables. Do not edit
=======
from pathlib import Path

if not Path("AP_Declination.h").is_file():
    raise OSError("Please run this tool from the AP_Declination directory")


def write_table(f,name, table):
    '''write one table'''
    f.write("const float AP_Declination::%s[%u][%u] = {\n" %
                (name, NUM_LAT, NUM_LON))
    for i in range(NUM_LAT):
        f.write("    {")
        for j in range(NUM_LON):
            f.write("%.5ff" % table[i][j])
            if j != NUM_LON-1:
                f.write(",")
        f.write("}")
        if i != NUM_LAT-1:
            f.write(",")
        f.write("\n")
    f.write("};\n\n")




if __name__ == '__main__':

    date = datetime.datetime.now()
   # date = datetime.date(2018,2,20)

    SAMPLING_RES = 10
    SAMPLING_MIN_LAT = -90
    SAMPLING_MAX_LAT = 90
    SAMPLING_MIN_LON = -180
    SAMPLING_MAX_LON = 180

    lats = np.arange(SAMPLING_MIN_LAT, SAMPLING_MAX_LAT+SAMPLING_RES, SAMPLING_RES)
    lons = np.arange(SAMPLING_MIN_LON, SAMPLING_MAX_LON+SAMPLING_RES, SAMPLING_RES)

    NUM_LAT = lats.size
    NUM_LON = lons.size

    intensity_table = np.empty((NUM_LAT, NUM_LON))
    inclination_table = np.empty((NUM_LAT, NUM_LON))
    declination_table = np.empty((NUM_LAT, NUM_LON))

    for i,lat in enumerate(lats):
        for j,lon in enumerate(lons):
            mag = igrf.igrf(date, glat=lat, glon=lon, alt_km=0., isv=0, itype=1)
            intensity_table[i][j] = mag.total/1e5
            inclination_table[i][j] = mag.incl
            declination_table[i][j] = mag.decl

    with open("tables.cpp", 'w') as f:
        f.write('''// this is an auto-generated file from the IGRF tables. Do not edit
>>>>>>> upstream/master
// To re-generate run generate/generate.py

#include "AP_Declination.h"

''')

tfile.write('''
const float AP_Declination::SAMPLING_RES = %u;
const float AP_Declination::SAMPLING_MIN_LAT = %u;
const float AP_Declination::SAMPLING_MAX_LAT = %u;
const float AP_Declination::SAMPLING_MIN_LON = %u;
const float AP_Declination::SAMPLING_MAX_LON = %u;

''' % (SAMPLING_RES,
       SAMPLING_MIN_LAT,
       SAMPLING_MAX_LAT,
       SAMPLING_MIN_LON,
       SAMPLING_MAX_LON))


def write_table(name, table):
    '''write one table'''
    tfile.write("const float AP_Declination::%s[%u][%u] = {\n" %
                (name, NUM_LAT, NUM_LON))
    for i in range(NUM_LAT):
        tfile.write("    {")
        for j in range(NUM_LON):
            tfile.write("%.5f" % table[i][j])
            if j != NUM_LON-1:
                tfile.write(",")
        tfile.write("}")
        if i != NUM_LAT-1:
            tfile.write(",")
        tfile.write("\n")
    tfile.write("};\n\n")


write_table('declination_table', declination_table)
write_table('inclination_table', inclination_table)
write_table('intensity_table', intensity_table)


tfile.close()
