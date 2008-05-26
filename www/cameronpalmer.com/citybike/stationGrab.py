#!/usr/bin/python
# coding=utf-8

import urllib2
from BeautifulSoup import BeautifulSoup
import re

def clean(sarr):
	"""Takes a string and removes any HTML Tags"""
	return sarr 

page = urllib2.urlopen("http://dynamisch.citybikewien.at/s_liste.php")
soup = BeautifulSoup(page)

START = 1
DIST = 0
NUM = 0
PIC = 0
NAME = 0
DESC = 0
STATUS = 0

station_number = None
station_name = None
station_desc = None
station_pic = None
max_bikes = 0
free_bikes = 0
free_boxes = 0

startre = re.compile(r'height="19"')
datare = re.compile(r'height="64"')
tablere = re.compile(r'<table [^>]*>')
distre = re.compile(r'(\d+)\.\s+BEZIRK')
numre = re.compile(r'(\d+)')
picre = re.compile(r'')
namere = re.compile(r'height="12px"')
descre = re.compile(r'')
statusre = re.compile(r'<b>Räder gesamt: </b>(\d+)<b> / Freie Räder: </b>(\d+)<b> / Freie Bikeboxen: </b>(\d+)')

# tr
# 	td height=19
#	td font stationnr
# tr
#	td height=64
#	td a href stationpic
#	td
#	td valign=top
#		table
#			tr
#				td p strong stationname
#				td p stationdesc
#				td p b gesamt number b freie number b boxen number

for td in soup.body.findAll('td'):
	if START == 1:
		if startre.search(str(td)):
			START = 0
			NUM = 1
			continue
	elif NUM == 1:
		if numre.search(str(td)):
			print "Station number:", td.font.string
			NUM = 0
			DATA = 1
			continue
	elif DATA == 1:
		if datare.search(str(td)):
			DATA = 0
			PIC = 1
			continue
	elif PIC == 1:
		if td.a:
			href = td.a['href']
			url = href.split(',')[0]
			url = url.replace('javascript:MM_openBrWindow(\'../../', 'http://dynamisch.citybikewien.at/')
			url = url.replace('%2F', '/')
			url = url.rstrip('\'')
			print "Station picture:", url
			PIC = 0
			NAME = 1  
			continue
	elif NAME == 1:
		if tablere.search(str(td)):
			continue
		elif td.p:
			print "Station name:", td.p.strong.string
			NAME = 0
			DESC = 1
			continue
	elif DESC == 1:
		if td.p:
			print "Station description:", clean(td.p.nextSibling.renderContents())
			DESC = 0
			STATUS = 1
			continue
	elif STATUS == 1:
		if td.p:
			m = statusre.search(str(td.p))
			print "Station status:"
			print "\tAvailable Bikes:", m.group(2)
			print "\tEmpty Spaces:", m.group(3)
			print "\tTotal Capacity:", m.group(1)
			print
			STATUS = 0
			START = 1
