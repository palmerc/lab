#!/usr/bin/env python

__author__ = "Cameron Palmer"
__copyright__ = "Copyright 2005, Cameron Palmer"
__version__ = "$Rev$"
__license__ = "GPL"

import os, glob, sys, re, string

#dirprefix = '../data/'
dirprefix = '/var/data/www/unt.cameronpalmer.com/data/'

buildings = {
    'ART': 'Art Building',
    'ATHC': 'Athletic Center',
    'ATH': 'Athletic Office Building',
    'AUDB': 'Auditorium Building',
    'BAIN': 'Bain Hall',
    'BIOL': 'Biology Building',
    'BRUC': 'Bruce Hall',
    'BUSI': 'Business Administration Building',
    'CHEM': 'Chemistry Building',
    'CHIL': 'Chilton Hall',
    'CLAR': 'Clark Hall',
    'COL': 'Coliseum',
    'CINN': 'College Inn',
    'CRUM': 'Crumley Hall',
    'CURY': 'Curry Hall',
    'DALL': 'Dallas Campus',
    'ESSC': 'Eagle Student Services Center',
    'ENV': 'Environmental Education, Science & Technology',
    'PLAN': 'Facilities Complex',
    'STAD': 'Fouts Field',
    'ROWF': 'Fraternity Row',
    'GATE': 'Gateway Center',
    'GAB': 'General Academic Building',
    'CHAP': 'Goolsby Chapel',
    'HKRY': 'Hickory Hall',
    'HIGH': 'Highland Hall',
    'ADMN': 'Hurley Administration Building',
    'INET': 'Internet Course',
    'ISB': 'Information Sciences Building',
    'MGYM': 'Ken Bahnsen Gym',
    'KEND': 'Kendall Hall',
    'KERR': 'Kerr Hall',
    'LANG': 'Language Building',
    'LANX': 'Library Annex',
    'LYCM': 'Lyceum',
    'MAPL': 'Maple Hall',
    'MARQ': 'Marquis Hall',
    'MAST': 'Masters Hall',
    'MHA': 'Matthews Hall Annex',
    'MATT': 'Matthews Hall',
    'MCON': 'McConnell Hall',
    'MGOG': 'Mean Green Office Complex and Gym',
    'MGSG': 'Mean Green Softball and Golf Practice Facility',
    'MOZA': 'Mozart Square',
    'PAC': 'Murchison Performing Arts Center',
    'MUSA': 'Music Annex',
    'MUSI': 'Music Building',
    'MPN': 'Music Practice North',
    'MPS': 'Music Practice South',
    'OSHA': 'Oak Street Hall Annex',
    'OSH': 'Oak Street Hall',
    'PARK': 'Parking Garage',
    'PACX': 'Performing Arts Center Annex',
    'PEB': 'Physical Education Building',
    'FIEL': 'Physical Education Field',
    'PHYS': 'Physics Building',
    'PDI': 'Professional Development Institute',
    'RTFP': 'Radio, TV, Film & Performing Arts Building',
    'NCAM': 'Research Park',
    'SFE': 'Santa Fe Square',
    'SRB': 'Science Research Building',
    'SCOU': 'Scoular Hall',
    'PAV': 'Shrader Pavilion',
    'ROWS': 'Sorrority Row',
    'SPHS': 'Speech and Hearing Clinic',
    'WGHT': 'Sports Medicine/Fitness Facility',
    'STOV': 'Stovall Hall',
    'SAAC': 'Student Athlete Academic Center',
    'HEAL': 'Student Health & Wellness Center',
    'RECS': 'Student Recreation Center',
    'POLI': 'Sullivant Public Safety Center',
    'WTEN': 'Tennis Courts',
    'TH': 'Terrill Hall',
    'TRAD': 'Traditions Hall',
    'USB': 'University Services Building',
    'UU': 'University Union',
    'VICT': 'Victory Hall',
    'TENN': 'Waranch Tennis Complex',
    'WEST': 'West Hall',
    'LIBR': 'Willis Library',
    'SOCR': 'Women’s Soccer Field',
    'WH': 'Wooten Hall',
}

def parsetxt(inputtxt):
   """
   CSV Description:
   TERM,DEPT,COURSENUM,COURSETITLE,SECTION,REGNUM,CREDITTYPE,CREDITHOURS,DAYS,STARTTIME,ENDTIME,CLASSROOM,PROF,NOTES
   """

   docstart = re.compile(r'^\s*Updated:\s+(?P<updatedate>\d{1,2}/\d{1,2}/\d{4})' \
                        r'\s+(?P<updatetime>\d{1,2}:\d{1,2}:\d{1,2}AM|PM)' \
                        r'\s+Term:(?P<term>\d{4})' \
                        r'\s+(?P<session>Spring|Summer|Fall)\s+(?P<year>\d+)$')    
   course = re.compile(r'^\s*(?P<coursedept>[A-Z]{3,4})\s(?P<coursenumber>\d{4})\s\s+(?P<coursetitle>.{1,30})\s*')
   section = re.compile(r'^\s*(?P<section>\d{3})\s+\((?P<regcode>\d+)\)' \
                        r'\s+(?P<type>CRE|LAB|REC)\s+(?P<credits>[V\d.]+)\s*')
   days = re.compile(r'\s(?P<days>[MTWRFSU]+)\s')
   times = re.compile(r'\s(?P<starttime>\d{2}:\d{2}\s(?:am|pm))' \
                        r'-(?P<endtime>\d{2}:\d{2}\s(?:am|pm))\s')
   classroom = re.compile(r'\s(?P<classroom>INET|(?:[A-Z]+\s[0-9]+))\s')
   instructor = re.compile(r'\s\s+(?P<instructor>[A-Z]{0,1}[a-z-\']*\s*[A-Z]{0,1}[a-z-\']*\s*[A-Z]{0,1}[a-z-\']*)\s*$')
      stack = []
   dept = None
   outputtxt = 'TERM,DEPT,COURSENUM,COURSETITLE,SECTION,REGNUM,CREDITTYPE,CREDITHOURS,DAYS,STARTTIME,ENDTIME,CLASSROOM,PROF,NOTES\n'
   for line in inputtxt:
      line = line.strip()
      docmatch = docstart.match(line)
      coursematch = course.match(line)
      sectionmatch = section.match(line)
      daysmatch = days.search(line)
      timesmatch = times.search(line)
      classroommatch = classroom.search(line)
      instructormatch = instructor.search(line)
      if docmatch:
         term = docmatch.group('term')
         continue

      elif coursematch:
         dept = coursematch.group('coursedept')
         number = coursematch.group('coursenumber')
         title = coursematch.group('coursetitle')
         continue

      if sectionmatch:
         stack.append(term)
         stack.append(dept)
         stack.append(number)
         stack.append(title)
         
         stack.append(sectionmatch.group('section'))
         stack.append(sectionmatch.group('regcode'))
         stack.append(sectionmatch.group('type'))
         stack.append(sectionmatch.group('credits'))

         if daysmatch:
            stack.append(daysmatch.group('days'))
         else:
            stack.append('')

         if timesmatch:
            stack.append(timesmatch.group('starttime'))
            stack.append(timesmatch.group('endtime'))
         else:
            stack.extend(['', ''])

         if classroommatch:
            stack.append(classroommatch.group('classroom'))
         else:
            stack.append('')

         if instructormatch:
            stack.append(instructormatch.group('instructor'))
         else:
            stack.append('')

         stack.append('')
            
         outputtxt = outputtxt + string.join(stack, ',') + '\n'
         stack = []       
    
   return outputtxt

def txttocsv(txtfile, csvfile=None):
   if os.path.isdir(txtfile):
      filelist = findtxts(txtfile)
   else:
      filelist = [txtfile]

   if csvfile:
      datadir = csvfile
   else:
      datadir = dirprefix + 'csv/'

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

def pdftotxt(pdffile, txtfile=None):
   """Convert the PDF versions of the schedule to TXT files"""

   if os.path.isdir(pdffile):
       filelist = findpdfs(pdffile)
   else:
       filelist = [pdffile]

   if txtfile and os.path.isdir(txtfile):
      datadir = txtfile
   else:
      datadir = dirprefix + 'txt/'

   for origfile in filelist:
      txtfile = re.split(r'[/\\]', origfile)[-1]
      semesdir = re.split(r'[/\\]', origfile)[-2]
      txtfile = txtfile.replace('.pdf', '.txt')
      if not os.path.exists(datadir + semesdir):
         try:
            os.makedirs(datadir + semesdir, 0755)
         except OSError, e:
            raise e
      txtfile = datadir + semesdir + '/' + txtfile          
            
      handle = os.popen('pdftotext -layout %s %s' % (origfile, txtfile))
      handle.close()
   return txtfile
    
if __name__ == '__main__':
    """Convert the PDF versions of the schedule to TXT files"""

    if len(sys.argv) > 1:
        pdffile =  sys.argv[1].strip()
    else:
        pdffile = dirprefix + 'pdf/'

    textfile = pdftotxt(pdffile)
    #textfile = dirprefix + 'txt/1061/mathematics_1061.txt'
    textfile = dirprefix + 'txt/'
    csvfile = txttocsv(textfile)
