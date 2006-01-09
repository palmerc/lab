#!/usr/bin/env python

__author__ = "Cameron Palmer"
__copyright__ = "Copyright 2005, Cameron Palmer"
__version__ = "$Rev$"
__license__ = "GPL"

import os, glob, sys, re, string

def parsetxt(inputtxt):
    """
    CSV Description:
    TERM,DEPT,COURSENUM,COURSETITLE,SECTION,REGNUM,CREDITTYPE,CREDITHOURS,DAYS,STARTTIME,ENDTIME,CLASSROOM,PROF,NOTES
    """

    docstart = re.compile(r'^\s*Updated:\s+(?P<updatedate>\d{1,2}/\d{1,2}/\d{4})' \
                          r'\s+(?P<updatetime>\d{1,2}:\d{1,2}:\d{1,2}AM|PM)' \
                          r'\s+Term:(?P<term>\d{4})' \
                          r'\s+(?P<session>Spring|Summer|Fall)\s+(?P<year>\d+)$')
    course = re.compile(r'^\s*(?P<coursedept>[A-Z]{3,4})\s(?P<coursenumber>\d{4})\s+(?P<coursetitle>.+)$')
    regsection = re.compile(r'^\s*(?P<section>\d{3})\s+\((?P<regcode>\d+)\)' \
                            r'\s+(?P<type>CRE|LAB|REC)\s+(?P<credits>[\d.]+)' \
                            r'\s+(?P<days>[MTWRFSU]+)\s+(?P<starttime>\d{2}:\d{2}\s+(?:am|pm))' \
                            r'-(?P<endtime>\d{2}:\d{2}\s+(?:am|pm))\s+(?P<classroom>[A-Z]+\s[0-9]*)' \
                            r'\s*(?P<instructor>.*)$')
    inetsection = re.compile(r'^\s*(?P<section>\d{3})\s+\((?P<regcode>\d+)\)' \
                            r'\s+(?P<type>CRE|LAB|REC)\s+(?P<credits>[\d.]+)' \
                            r'\s+(?P<classroom>INET)\s*(?P<instructor>.*)$')
    specsection = re.compile(r'^\s*(?P<section>\d{3})\s+\((?P<regcode>\d+)\)' \
                            r'\s+(?P<type>CRE|LAB|REC)\s+(?P<credits>[V\d.]+)' \
                            r'\s*(?P<instructor>[A-Z]*.*)$')
    stack = []
    dept = None
    #i = 0
    outputtxt = 'TERM,DEPT,COURSENUM,COURSETITLE,SECTION,REGNUM,CREDITTYPE,CREDITHOURS,DAYS,STARTTIME,ENDTIME,CLASSROOM,PROF,NOTES\n'
    for line in inputtxt:
        line = line.strip()
        docmatch = docstart.match(line)
        coursematch = course.match(line)
        regsectionmatch = regsection.match(line)
        inetsectionmatch = inetsection.match(line)
        specsectionmatch = specsection.match(line)
        if docmatch:
            term = docmatch.group('term')
            continue
            #print 'DOCMATCH: %s,%s,%s,%s %s' % docmatch.groups()
        elif coursematch:
            dept = coursematch.group('coursedept')
            number = coursematch.group('coursenumber')
            title = coursematch.group('coursetitle')
            continue
            #print 'COURSE: %s,%s,%s' % coursematch.groups()

        if not len(stack) and dept:
            stack.extend([term,dept,number,title])

        if regsectionmatch:
            #i = i+1
            stack.extend(regsectionmatch.groups())
            stack.append('')
            outputtxt = outputtxt + string.join(stack, ',') + '\n'
            stack = []
        elif inetsectionmatch:
            #i = i+1
            stack.append(inetsectionmatch.group('section'))
            stack.append(inetsectionmatch.group('regcode'))
            stack.append(inetsectionmatch.group('type'))
            stack.append(inetsectionmatch.group('credits'))
            stack.extend(['','',''])
            stack.append(inetsectionmatch.group('classroom'))
            stack.append(inetsectionmatch.group('instructor'))
            stack.append('')
            outputtxt = outputtxt + string.join(stack, ',') + '\n'
            stack = []
        elif specsectionmatch:
            #i = i+1
            stack.append(specsectionmatch.group('section'))
            stack.append(specsectionmatch.group('regcode'))
            stack.append(specsectionmatch.group('type'))
            stack.append(specsectionmatch.group('credits'))
            stack.extend(['','','',''])
            stack.append(specsectionmatch.group('instructor'))
            stack.append('')
            outputtxt = outputtxt + string.join(stack, ',') + '\n'
            stack = []
        else:
            stack = []
            #print 'NOMATCH: %s' % line
    
    #print i 
    return outputtxt

def txttocsv(txtfile, csvfile=None):
    if os.path.isdir(txtfile):
        filelist = findtxts(txtfile)
    else:
        filelist = [txtfile]

    if csvfile and os.path.isdir(csvfile):
        datadir = csvfile
    else:
        #datadir = '../data/txt/'
        datadir = '/var/data/www/unt.cameronpalmer.com/data/csv/'

    if csvfile and os.path.isfile(txtfile):
        input = open(txtfile, 'rb')
        output = open(outfile, 'w')
        outputtxt = parsetxt(input.readlines())
        output.write(outputtxt)
        input.close()
        output.close()
    else:
        for origfile in filelist:
            csvfile = re.split(r'[/\\]', origfile)[-1]
            semesdir = re.split(r'[/\\]', origfile)[-2]
            csvfile = csvfile.replace('.txt', '.csv')
            if not os.path.exists(datadir + semesdir):
            	try:
             		os.makedirs(datadir + semesdir, 0755)
            	except OSError, e:
            		raise e
            csvfile = datadir + semesdir + '/' + csvfile
            input = open(origfile, 'rb')
            output = open(csvfile, 'w')
            outputtxt = parsetxt(input.readlines())
            output.write(outputtxt)
            input.close()
            output.close()
            

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

def pdftotext(pdffile, textfile=None):
    """Convert the PDF versions of the schedule to TXT files"""

    if os.path.isdir(pdffile):
        filelist = findpdfs(pdffile)
    else:
        filelist = [pdffile]

    if textfile and os.path.isdir(textfile):
        datadir = textfile
    else:
        #datadir = '../data/txt/'
        datadir = '/var/data/www/unt.cameronpalmer.com/data/txt/'

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
             		os.makedirs(datadir + semesdir, 0755)
            	except OSError, e:
            		raise e
            textfile = datadir + semesdir + '/' + textfile          
            
            handle = os.popen('pdftotext -layout %s %s' % (origfile, textfile))
            handle.close()
    return textfile
    
if __name__ == '__main__':
    """Convert the PDF versions of the schedule to TXT files"""

    if len(sys.argv) > 1:
        pdffile =  sys.argv[1].strip()
    else:
        #pdffile = '../data/pdf/'
        pdffile = '/var/data/www/unt.cameronpalmer.com/data/pdf/'

    textfile = pdftotext(pdffile)
    #textfile = '../data/txt/1061/accounting_1061.txt'
    textfile = '/var/data/www/unt.cameronpalmer.com/data/txt/'
    csvfile = txttocsv(textfile)