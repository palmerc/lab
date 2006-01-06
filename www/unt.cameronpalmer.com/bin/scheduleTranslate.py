#!/usr/bin/env python

__author__ = "Cameron Palmer"
__copyright__ = "Copyright 2005, Cameron Palmer"
__version__ = "$Rev$"
__license__ = "GPL"

import os, glob, sys, re

def findpdfs(directory):
    # http://effbot.org/librarybook/os-path.htm
    stack = [directory]
    files = []
        
    while stack:
        directory = stack.pop()
        for file in os.listdir(directory):
            fullname = os.path.join(directory, file)
            if file.endswith('.pdf'):
                files.append(fullname)
            if os.path.isdir(fullname) and not os.path.islink(fullname):
                stack.append(fullname)
    return files

def pdftotext(pdffile, textfile=None):
    """Convert the PDF versions of the schedule to TXT files"""

    if os.path.isdir(pdffile):
        filelist = findpdfs(pdffile)
    else:
        filelist = [pdffile]

    if textfile and os.path.isdir(textfile):
        datadir = textfile
    else:
        datadir = '../data/txt/'

    if textfile and os.path.isfile(textfile):
        handle = os.popen('pdftotext -layout %s %s' % (pdffile, textfile))
        handle.close()
    else:
        for origfile in filelist:
            textfile = re.split(r'[/\\]', origfile)[-1]
            semesdir = re.split(r'[/\\]', origfile)[-2]
            textfile = textfile.replace('.pdf', '.txt')
            if not os.path.exists(datadir + semesdir):
            	try:
             		os.makedirs(datadir + semesdir, 0644)
            	except OSError, e:
            		raise e
            textfile = datadir + semesdir + textfile          
            
            handle = os.popen('/usr/local/bin/pdftotext -layout %s %s' % (pdffile, textfile))
            handle.close()
    
if __name__ == '__main__':
    """Convert the PDF versions of the schedule to TXT files"""

    if len(sys.argv) > 1:
        pdffile =  sys.argv[1].strip()
    else:
        pdffile = '../data/pdf/'

    pdftotext(pdffile)