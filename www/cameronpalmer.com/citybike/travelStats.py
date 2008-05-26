#!/usr/bin/python
# coding=utf-8

import re
import urllib2
from BeautifulSoup import BeautifulSoup

page = urllib2.urlopen("http://dynamisch.citybikewien.at/right.php")
soup = BeautifulSoup(page)

# td jahreskm1.gif
# td kilometers_ytd
# td tageskm1.gif
# td kilometers_today
# td fahrendebikes1.gif

YEAR = 0
DAY = 0
OUT = 0
yearkmre = re.compile(r'jahreskm1.gif')
todaykmre = re.compile(r'tageskm1.gif')
bikesoutre = re.compile(r'fahrendebikes1.gif')
counterre = re.compile(r'img/counter_(\d+).gif')
decimalre = re.compile(r'img/counter_r_(\d+).gif')

for td in soup.body.findAll('td'):
	if YEAR == 1:
		year = 0
		for img in td.img.findNextSiblings():
			year *= 10
			if counterre.search(str(img)):
				m = counterre.search(str(img))
				year += int(m.group(1))
		print "Bike kilometers this year", year
		YEAR = 0
	elif DAY == 1:
		day = 0
		for img in td.img.findNextSiblings():
			if decimalre.search(str(img)):
				m = decimalre.search(str(img))
				day += int(m.group(1)) * .10
			else:
				day *= 10
			if counterre.search(str(img)):
				m = counterre.search(str(img))
				day += int(m.group(1))
		print "Bike kilometers today:", day
		DAY = 0
	elif OUT == 1:
		out = 0
		for img in td.img.findNextSiblings():
			out *= 10
			if counterre.search(str(img)):
				m = counterre.search(str(img))
				out += int(m.group(1))
		print "Bikes in use:", out
		OUT = 0

	if yearkmre.search(str(td)):
		YEAR = 1
	elif todaykmre.search(str(td)):
		DAY = 1
	elif bikesoutre.search(str(td)):
		OUT = 1
