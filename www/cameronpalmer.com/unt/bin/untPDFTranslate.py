#!/usr/bin/env python

__author__ = "Cameron Palmer"
__copyright__ = "Copyright 2008, Cameron Palmer"
__version__ = "$Rev$"
__license__ = "GPL"

import os, glob, sys, re, string

bindir = None
datadir = None

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
      csvdir = datadir + 'csv/'

   for origfile in filelist:
      txtfile = re.split(r'[/\\]', origfile)[-1]
      csvfile = txtfile
      semesdir = re.split(r'[/\\]', origfile)[-2]
      txtfile = txtfile.replace('.pdf', '.txt')
      csvfile = txtfile.replace('.pdf', '.csv')
      if not os.path.exists(txtdir + semesdir):
         try:
            os.makedirs(txtdir + semesdir, 0755)
         except OSError, e:
            raise e
      if not os.path.exists(csvdir + semesdir):
         try:
            os.makedirs(csvdir + semesdir, 0755)
         except OSError, e:
            raise e
      txtfile = txtdir + semesdir + '/' + txtfile
      csvfile = csvdir + semesdir + '/' + csvfile          
            
      pdftoxy = bindir + 'pdftoxy'
      handle = os.popen(pdftoxy + ' %s | ./untPDFToTXT.py > %s' % (origfile, txtfile))
      handle.close()
      handle = os.popen('./untTXTToCSV.py %s > %s' % (txtfile, csvfile))
      handle.close()
   return txtfile
    
if __name__ == '__main__':
    """Convert the PDF versions of the schedule to TXT files"""

    if len(sys.argv) > 1:
        basedir =  sys.argv[1].strip()
    else:
        print 'Usage:', sys.argv[0], 'basedir'
    pdffile = basedir + '/data/pdf/'
    bindir = basedir + '/bin/'
    datadir = basedir + '/data/'
    textfile = pdftotxt(pdffile)
