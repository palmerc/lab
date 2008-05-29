#!/usr/bin/python
# coding=utf-8

"""
This program is supposed to grab the CityBike Wien bike station status information.
Pass either the --report or --csv option to the program. Defaults to --csv.
Usage: ./stationGrab [--report or --csv]
"""

from urllib2 import Request, urlopen, HTTPError, URLError
from BeautifulSoup import BeautifulSoup
import re
import sys
import getopt
import pickle

stations = {}
citybike_server = "http://dynamisch.citybikewien.at/"
status_url = citybike_server + "s_liste.php"
img_url = citybike_server + "include/r4_get_data.php?url=terminal/cont/img/"
spacere = re.compile(r'\s\s+')

def toXML():
	"""Dump the station information XML"""
	pass

def toCSV():
	"""Dump the station information CSV"""
	f = open("status.csv", "w")
	print u'number;name;desc;pic;free;empty;capacity'
	for key in sorted(stations.keys(), lambda x, y: x-y):
		station_number = str(key)
		station_name = stations[key]["name"]
		station_desc = stations[key]["description"]
		station_pic = stations[key]["picture"]
		free_bikes = str(stations[key]["available"])
		free_boxes = str(stations[key]["empty"])
		max_bikes = str(stations[key]["capacity"])
		tmpArr = [station_number, station_name, station_desc, station_pic, free_bikes, free_boxes, max_bikes]
		csvString = u';'.join(tmpArr) + '\n'
		f.write(csvString.encode('utf-8'))
	f.close()

def toString():
	"""Dump the station information"""
	for key in sorted(stations.keys(), lambda x, y: x-y):
		station_number = str(key)
		station_name = stations[key]["name"]
		station_desc = stations[key]["description"]
		station_pic = stations[key]["picture"]
		free_bikes = str(stations[key]["available"])
		free_boxes = str(stations[key]["empty"])
		max_bikes = str(stations[key]["capacity"])

		print u'Station:', station_number, station_name
		print u'Description:', station_desc
		print u'Picture:', station_pic
		print u'Available Bikes:', free_bikes + ',',
		print u'Empty Spaces:', free_boxes + ',',
		print u'Total Capacity:', max_bikes
		print


def descfix(L):
	s = u'; '.join(map(lambda x: x.strip(), L))
	s = re.sub(spacere, ' ', s)
	return s

def process(page):
	START = 1
	DIST = 0
	NUM = 0
	PIC = 0
	NAME = 0
	DESC = 0
	STATUS = 0
	
	station_number = 0 
	station_name = None
	station_desc = None
	station_pic = None
	max_bikes = 0
	free_bikes = 0
	free_boxes = 0
	
	datare = re.compile(r'height="64"')
	distre = re.compile(r'(\d+)\.\s+BEZIRK')
	namere = re.compile(r'height="12px"')
	numre = re.compile(r'(\d+)')
	startre = re.compile(r'height="19"')
	statusre = re.compile(r'<b>Räder gesamt: </b>(\d+)<b> / Freie Räder: </b>(\d+)<b> / Freie Bikeboxen: </b>(\d+)')
	tablere = re.compile(r'<table [^>]*>')
	
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

	soup = BeautifulSoup(page)
	
	for td in soup.body.findAll('td'):
		if START == 1:
			if startre.search(repr(td)):
				START = 0
				NUM = 1
				continue
		elif NUM == 1:
			if numre.search(repr(td)):
				station_number = int(td.font.string)
				NUM = 0
				DATA = 1
				continue
		elif DATA == 1:
			if datare.search(repr(td)):
				DATA = 0
				PIC = 1
				continue
		elif PIC == 1:
			if td.a:
				href = td.a['href']
				url = href.split(',')[0]
				url = url.replace('%2F', '/')
				url = url.replace('javascript:MM_openBrWindow(\'../../include/r4_get_data.php?url=terminal/cont/img/', '')
				url = url.rstrip('\'')
				station_pic = url
				PIC = 0
				NAME = 1  
				continue
		elif NAME == 1:
			if tablere.search(repr(td)):
				continue
			elif td.p:
				station_name = re.sub(spacere, ' ', td.p.strong.string)
				NAME = 0
				DESC = 1
				continue
		elif DESC == 1:
			if td.p:
				station_desc = descfix(td.p.nextSibling.fetchText(text=True))
				DESC = 0
				STATUS = 1
				continue
		elif STATUS == 1:
			if td.p:
				m = statusre.search(repr(td.p))
				free_bikes = int(m.group(2))
				free_boxes = int(m.group(3))
				max_bikes = int(m.group(1))
				STATUS = 0
				FINAL = 1
		elif FINAL == 1:
			stations[station_number] = {}
			stations[station_number]["name"] = station_name
			stations[station_number]["description"] = station_desc
			stations[station_number]["picture"] = station_pic
			stations[station_number]["available"] = free_bikes
			stations[station_number]["empty"] = free_boxes
			stations[station_number]["capacity"] = max_bikes
			FINAL = 0
			START = 1

class Usage(Exception):
	def __init__(self, msg):
		self.msg = msg

def main(argv=None):
	if argv is None:
		argv = sys.argv
	report = 0
	csv = 1
	kosher = 0

	try:
		try:
			opts, args = getopt.getopt(sys.argv[1:], '', ['file', 'help', 'report', 'csv', 'kosher'])
		except getopt.error, msg:
			raise Usage(msg)
	except Usage, err:
		print >>sys.stderr, err.msg
		print >>sys.stderr, "for help use --help"
		return 2

	for o, a in opts:
		csv = 0
		if o in ('-h', '--help'):
			print __doc__
			sys.exit(0)
		elif o in ('-r', '--report'):
			report = 1
		elif o in ('-c', '--csv'):
			csv = 1

		if o in ('-k', '--kosher'):
			kosher = 1

	page = None
	req = Request(status_url)
	try:
		page = urlopen(req)
	except HTTPError, e:
		print e
		return 2
	except URLError, e:
		print e
		return 2

	process(page)
	if report == 1:
		toString()
	elif csv == 1:
		toCSV()

	if kosher == 1:
		f = open('kosher', 'w')
		pickle.dump(stations, f)
		f.close

if __name__ == "__main__":
	sys.exit(main())
