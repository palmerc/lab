#!/usr/bin/env python

__author__ = "Cameron Palmer"
__copyright__ = "Copyright 2008, Cameron Palmer"
__version__ = "$Rev$"
__license__ = "GPL"

import re
import sys

rspace = re.compile(r"^(\s+)", re.DOTALL)
rterm = re.compile(r"Term:(\d+) (\w+) (\d+)$", re.DOTALL)
rdept = re.compile(r"(\w{4})/(.+)$", re.DOTALL)
rcourse = re.compile(r"([A-Z]{4}) (\d{4}) (.+)$", re.DOTALL)
rsection = re.compile(r"(\d{3}) \((\d{5})\) (CRE|REC|LAB) (\d\.\d|V) (.+)$", re.DOTALL)

state = "TERM"
spaces = 0
nline = ""

term = ""
semester = ""
year = ""
cdept = ""
ddept = ""
name = ""
number = ""
title = ""
section = ""
code = ""
type = ""
hours = ""
rest = ""
note = ""

def mterm(line):
	global state, term, semester, year
	x = line.strip()
	if rterm.search(x):
		c = rterm.search(x)
		term = c.group(1)
		semester = c.group(2)
		year = c.group(3)
		state = "DEPT"

def mdept(line):
	global state, ddept, name
	x = line.strip()
	if rdept.search(x):
		c = rdept.search(x)
		ddept = c.group(1)
		name = c.group(2)
		state = "COURSE"

def mcourse(line):
	global state, cdept, number, title

	x = line.strip()
	if rcourse.search(x):
		c = rcourse.search(x)
		cdept = c.group(1)
		if cdept != ddept:
			print "Course department", cdept, "and", ddept, "department do not match."
			sys.exit(1)
		number = c.group(2)
		title = c.group(3)
		state = "SECTION"
	else:
		print "Error mcourse:", x
	
def msection(line):
	global state, section, code, type, hours, rest
	x = line.strip()
	if rsection.search(x):
		c = rsection.search(x)
		section = c.group(1)
		code = c.group(2)
		type = c.group(3)
		hours = c.group(4)
		rest = c.group(5)
		if rsection.search(nline):
			lprint()
			state = "SECTION"
		elif rcourse.search(nline):
			lprint()
			state = "COURSE"
		elif rdept.search(nline):
			lprint()
			state = "DEPT"
		elif spaces < len(rspace.search(nline).group(1)):
			state = "NOTE"
	
def mnote(line):
	global state, note
	x = line.strip()
	if rsection.search(nline):
		note += x
		lprint()
		note = ""
		state = "SECTION"
	elif rcourse.search(nline):
		note += x
		lprint()
		note = ""
		state = "COURSE"
	elif rdept.search(nline):
		note += x
		lprint()
		note = ""
		state = "DEPT"
	elif spaces == len(rspace.search(nline).group(1)):
		note += x
		state = "NOTE"

def lprint():
	parr = [term,semester,year,ddept,name,cdept,number,title,section,code,type,hours,rest,note]
	print ','.join(parr)

def machine(line):
	global spaces
	x = len(rspace.search(line).group(1))
	line = line.lstrip()
		
	if x != spaces:
		spaces = x
			
	if state == "TERM":
		mterm(line)
	elif state == "DEPT":
		mdept(line)
	elif state == "COURSE":
		mcourse(line)
	elif state == "SECTION":
		msection(line)
	elif state == "NOTE":
		mnote(line)
	else:
		print "State Error:", line
		
def main():
	global nline
	if (len(sys.argv) > 1):
		f = open(sys.argv[1], "rb")
	else:
		f = sys.stdin

	contents = f.read()
	line = " "
	for nline in contents.splitlines(True):
		machine(line)
		line = nline 	
	machine(line)

	f.close()

if __name__ == "__main__":
	main()
