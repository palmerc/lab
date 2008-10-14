#!/usr/bin/env python

__author__ = "Cameron Palmer"
__copyright__ = "Copyright 2008, Cameron Palmer"
__version__ = "$Rev$"
__license__ = "GPL"

import urllib2

url = 'http://www.unt.edu/pais/map/campusmap.htm'
dataDir = '/var/www/cameronpalmer.com/unt/data/buildings/'
csvFile = 'untBuildings.csv'

r = urllib2.urlopen(url)

d = r.read().splitlines(True)
buildings = eval(d[238][14:-5])

print buildings
