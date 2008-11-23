#!/usr/bin/env python

__author__ = "Cameron Palmer"
__copyright__ = "Copyright 2008, Cameron Palmer"
__version__ = "$Rev$"
__license__ = "GPL"

import re
import urllib2
from htmlentitydefs import name2codepoint

url = "http://web3.unt.edu/urcm/campusmap/buildings.cfm"

def utf8(s):
    return unicode(s, "utf-8")

def htmlentitydecode(s):
    return re.sub('&(%s);' % '|'.join(name2codepoint), 
            lambda m: unichr(name2codepoint[m.group(1)]), s)

bldgFromJson = re.compile("bldgFromJson\((.+)\);")

r = urllib2.urlopen(url)
x = r.read()

buildings = eval(bldgFromJson.search(x).group(1))

for building in buildings:
    print htmlentitydecode(utf8('"' + building['id'] + '","' + building['label'] + '"'))
print '"DAL1","Dallas Campus"'