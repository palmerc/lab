#!/usr/bin/env python

__author__ = "Cameron Palmer"
__copyright__ = "Copyright 2008, Cameron Palmer"
__version__ = "$Rev$"
__license__ = "GPL"

import urllib2, os, sys
sys.path.append("BeautifulSoup-2.1.1/")
from BeautifulSoup import BeautifulSoup

if __name__ == '__main__':
    """Download the PDF class schedules"""

    if len(sys.argv) > 1:
        basedir = sys.argv[1].strip()
    else:
        print 'Usage: sys.argv[0] basedir'
        sys.exit(1)
    base = 'http://essc.unt.edu/registrar/SOCbydept/'
    url = 'SOCbydeptA.htm'
    datadir = basedir + '/data/pdf/'
    html = urllib2.urlopen(base+url) # A file like object
    soup = BeautifulSoup(html)
    for anchor in soup('a'):
        link = anchor['href']
        if link.endswith('.pdf'):
            fileurl = base+link
            fileout = (datadir+link).replace('%20', '_').lower()
            directory = datadir+link.lower().split('/', 1)[0]
            if not os.path.exists(directory):
	        try:
                    os.mkdir(directory)

                except OSError, e:
                    if 'File Exists' in e.strerror:
                        raise e
                    else:
                        print e.strerror

            	
            try:
            	handle = os.popen('wget -q -O %s %s' % (fileout, fileurl))
            	handle.close()
            except OSError, e:
            	print e.strerror
