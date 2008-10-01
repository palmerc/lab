#!/usr/bin/env python

import re
import sys
from StringIO import StringIO

def main():
	if (len(sys.argv) > 1):
		f = open(sys.argv[1], "rb")
	else:
		f = sys.stdin
	rfile = re.compile(r"file=(\d+)", re.DOTALL)
	rpage = re.compile(r"page=(\d+)", re.DOTALL)
	rkey = re.compile(r"Updated:.+", re.DOTALL)
	rtext = re.compile(r"x=(\d+\.\d+) y=(\d+\.\d+) '(.+)'", re.DOTALL)

	currt = 0
	prevx = 0
	prevy = 0
	for line in f.readlines():
		if rfile.match(line):
			pass
		elif rpage.match(line):
			pass
		elif rtext.match(line):
			m = rtext.match(line)
			currx = int(float(m.group(1)))
			curry = int(float(m.group(2)))
			text = m.group(3)
			
			if rkey.match(text):
				prevx = currx
				spaced = prevy = curry	
				spaced /= 10
			
			if curry == prevy:
				sys.stdout.write(" " + text)
			else:
				sys.stdout.write("\n")
				count = (currx / 10) - spaced
				i = 0
				while i < count:
					sys.stdout.write("   ")	
					i += 1
				sys.stdout.write(text)
			prevx = currx
			prevy = curry
		else:
			pass
	f.close()

if __name__ == "__main__":
	main()
