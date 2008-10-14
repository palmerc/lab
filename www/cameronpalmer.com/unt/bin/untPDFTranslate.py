#!/usr/bin/env python

__author__ = "Cameron Palmer"
__copyright__ = "Copyright 2008, Cameron Palmer"
__version__ = "$Rev$"
__license__ = "GPL"

import os, glob, sys, re, string

basedir = '/var/www/cameronpalmer.com/unt/'
bindir = basedir + 'bin/'
datadir = basedir + 'data/'

def findtxts(directory):
    # http://effbot.org/librarybook/os-path.htm
    stack = [directory]
    files = []
        
    while stack:
        directory = stack.pop()
        for file in os.listdir(directory):
            fullname = os.path.join(directory, file)
            if file.endswith('.txt'):
                files.append(fullname)
            if os.path.isdir(fullname) and not os.path.islink(fullname):
                stack.append(fullname)
    return files

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

def pdftotxt(pdffile, txtfile=None):
   """Convert the PDF versions of the schedule to TXT files"""

   if os.path.isdir(pdffile):
       filelist = findpdfs(pdffile)
   else:
       filelist = [pdffile]

   if txtfile and os.path.isdir(txtfile):
      txtdir = txtfile
   else:
      txtdir = datadir + 'txt/'

   for origfile in filelist:
      txtfile = re.split(r'[/\\]', origfile)[-1]
      semesdir = re.split(r'[/\\]', origfile)[-2]
      txtfile = txtfile.replace('.pdf', '.txt')
      if not os.path.exists(txtdir + semesdir):
         try:
            os.makedirs(txtdir + semesdir, 0755)
         except OSError, e:
            raise e
      txtfile = txtdir + semesdir + '/' + txtfile          
            
      pdftoxy = bindir + 'pdftoxy'
      handle = os.popen(pdftoxy + ' %s | ./untPDFToTXT.py > %s' % (origfile, txtfile))
      handle.close()
   return txtfile
    
if __name__ == '__main__':
    """Convert the PDF versions of the schedule to TXT files"""

    if len(sys.argv) > 1:
        pdffile =  sys.argv[1].strip()
    else:
        pdffile = datadir + 'pdf/'

    textfile = pdftotxt(pdffile)
