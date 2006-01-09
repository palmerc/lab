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
    section = re.compile(r'^\s(?P<section>\d{3})\s+\((?P<regcode>\d+)\)' \
                            r'\s+(?P<type>CRE|LAB|REC)\s+(?P<credits>[V\d.]+)\s')
    days = re.compile(r'\s(?P<days>M{0,1}T{0,1}W{0,1}R{0,1}F{0,1}S{0,1}U{0,1})\s')
    times = re.compile(r'\s(?P<starttime>\d{2}:\d{2}\s(?:am|pm))' \
                            r'-(?P<endtime>\d{2}:\d{2}\s(?:am|pm))\s')
    classroom = re.compile(r'\s(?P<classroom>INET|(?:[A-Z]+\s[0-9]+))\s')
    instructor = re.compile(r'INET|[0-9 ]*(?P<instructor>[A-Za-z- ]+)$')

    stack = []
    dept = None
    #i = 0
    outputtxt = 'TERM,DEPT,COURSENUM,COURSETITLE,SECTION,REGNUM,CREDITTYPE,CREDITHOURS,DAYS,STARTTIME,ENDTIME,CLASSROOM,PROF,NOTES\n'
    for line in inputtxt:
        line = line.strip()
        docmatch = docstart.match(line)
        coursematch = course.match(line)
        sectionmatch = section.match(line)
        daysmatch = days.match(line)
        timesmatch = times.match(line)
        classroommatch = classroom.match(line)
        instructormatch = instructor.match(line)
        if docmatch:
            term = docmatch.group('term')
            continue

        elif coursematch:
            dept = coursematch.group('coursedept')
            number = coursematch.group('coursenumber')
            title = coursematch.group('coursetitle')
            continue

        if not len(stack) and dept:
            stack.extend([term,dept,number,title])

        if sectionmatch:
            stack.append(term)
            stack.append(coursedept)
            stack.append(coursenumber)
            stack.append(coursetitle)

            stack.append(sectionmatch.group('section'))
            stack.append(sectionmatch.group('regcode'))
            stack.append(sectionmatch.group('type'))
            stack.append(sectionmatch.group('credits'))
                        
            stack.append(daysmatch.group('days'))

            stack.append(timesmatch.group('starttime'))
            stack.append(timesmatch.group('endtime'))

            stack.append(classroommatch.group('classroom'))
            stack.append(instructormatch.group('instructor'))
            
            outputtxt = outputtxt + string.join(stack, ',') + '\n'
            stack = []
    
    return outputtxt

def txttocsv(txtfile, csvfile=None):
    if os.path.isdir(txtfile):
        filelist = findtxts(txtfile)
    else:
        filelist = [txtfile]

    if csvfile and os.path.isdir(csvfile):
        datadir = csvfile
    else:
        datadir = '../data/csv/'
        #datadir = '/var/data/www/unt.cameronpalmer.com/data/csv/'

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

    #textfile = pdftotext(pdffile)
    textfile = '../data/txt/1061/accounting_1061.txt'
    #textfile = '/var/data/www/unt.cameronpalmer.com/data/txt/'
    csvfile = txttocsv(textfile)
