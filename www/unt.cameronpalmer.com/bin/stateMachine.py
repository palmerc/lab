import os, re

class scheduleParser:
   beginblank = re.compile(r'^(\s+)')
   formfeed = re.compile(r'^\f')
   docstart = re.compile(r'^Updated:\s+(?P<updatedate>\d{1,2}/\d{1,2}/\d{4})' \
                  r'\s+(?P<updatetime>\d{1,2}:\d{1,2}:\d{1,2}AM|PM)' \
                  r'\s+Term:(?P<term>\d{4})' \
                  r'\s+(?P<session>Spring|Summer|Fall)\s+(?P<year>\d+)$')
   department = re.compile(r'^\s*(?P<coursedept>[A-Z]{3,4})/(?P<extddept>.+)$')
   course = re.compile(r'^\s*(?P<coursedept>[A-Z]{3,4})\s' \
                     r'(?P<coursenumber>\d{4})\s+(?P<coursetitle>.+)$')
   section = re.compile(r'^\s*(?P<section>\d{3})\s+\((?P<regcode>\d+)\)' \
                  r'\s+(?P<type>CRE|LAB|REC)\s+(?P<credits>[V\d.]+)')
   days = re.compile(r'\s(?P<days>[MTWRFSU]+)\s')
   times = re.compile(r'\s(?P<starttime>\d{2}:\d{2}\s(?:am|pm))' \
                  r'-(?P<endtime>\d{2}:\d{2}\s(?:am|pm))\s')
   classroom = re.compile(r'\s(?P<classroom>INET|(?:[A-Z]+\s[0-9]+))\s')
   instructor = re.compile(r'\s\s+(?P<instructor>[A-Z]{0,1}[a-z-\']*\s*[A-Z]{0,1}[a-z-\']*\s*[A-Z]{0,1}[a-z-\']*)\s*$')
    
   def __init__(self):
      ## Starting state
      self.state = 'DOCSTART'
      self.blankcount = 0
      self.courseblankcount = 80
      self.secblankcount = 80
      self.coursecount = 0
      self.seccount = 0
      
   def startDoc(self, line):
      docmatch = self.docstart.match(line)
      self.updatedate = docmatch.group('updatedate')
      self.updatetime = docmatch.group('updatetime')
      self.term = docmatch.group('term')
      self.session = docmatch.group('session')
      self.year = docmatch.group('year')
      self.state = 'DEPARTMENT'

   def startDepart(self, line):
      self.deptlist = line.strip().split('/')
      (self.dept, self.deptlong) = self.deptlist
      self.state = 'COURSE'

   def startCourse(self, line):
      blankmatch = self.beginblank.match(line)
      if blankmatch:
         (start, end) = blankmatch.span()
         currentblankcount = end - start
      else:
         currentblankcount = 0
            
      if currentblankcount <= self.courseblankcount:
         print line
         self.coursecount += 1
         self.courseblankcount = currentblankcount
         linematch = self.department.match(line)
         if linematch:
            print 'linematch'
            print linematch.group('coursedept') + linematch.group('coursenumber') \
                  + linematch.group('coursetitle')
      else:
         #print 'This line is not less than or equal to previous in spaces'
         pass
         
      # Transition to next state
      self.state = 'SECTION'

   def startSection(self, line):
      # The only reason I care about spaces is to identify notes
      blankmatch = self.beginblank.match(line)
      if blankmatch:
         (start, end) = blankmatch.span()
         self.secblankcount = end - start
      
      print line
      self.seccount += 1
      
      self.state = 'SECTION'
        
   def startNotes(self, line):
      blankmatch = self.beginblank.match(line)
      if blankmatch:
         (start, end) = blankmatch.span()
         currentblankcount = end - start
      
      self.state = 'NOTES'

testfile = './course_test.txt'

def find(directory, filetype):
    # http://effbot.org/librarybook/os-path.htm
    stack = [directory]
    files = []
        
    while stack:
        directory = stack.pop()
        for file in os.listdir(directory):
            fullname = os.path.join(directory, file)
            if file.endswith(filetype):
                files.append(fullname)
            if os.path.isdir(fullname) and not os.path.islink(fullname):
                stack.append(fullname)
    return files

f = open(testfile, 'rb')

sp = scheduleParser()

for line in f:
   # XPDF has this tendency to leave form feeds at the beginning of lines
   line = line.rstrip()
   line = line.lstrip('\f')
    
   ## Hello Mister State Machine
   if sp.state == 'DOCSTART':
      if line.strip() == '':
         #print 'BLANKLINE'
         continue
      elif sp.docstart.match(line): sp.startDoc(line)
      else:
         #print 'NOMATCH in DOCSTART'
         pass
        
   elif sp.state == 'DEPARTMENT':
      if line.strip() == '':
         #print 'BLANKLINE'
         continue
      elif sp.department.match(line): sp.startDepart(line)
      else:
         #print 'NOMATCH in DEPARTMENT'
         pass

   elif sp.state == 'COURSE':
      if line.strip() == '':
         #print 'BLANKLINE'
         continue
      elif sp.course.match(line): sp.startCourse(line)
      else:
         #print 'NOMATCH in COURSE'
         pass

   elif sp.state == 'SECTION':
      if line.strip() == '':
         #print 'BLANKLINE'
         continue
      elif sp.section.match(line):
         sp.state == 'SECTION'
         sp.startSection(line)
      elif sp.course.match(line):
         sp.state == 'COURSE'
         sp.startCourse(line)
      elif sp.department.match(line):
         sp.state == 'DEPARTMENT'
         sp.startDepart(line)
      #elif notes.match(line): startNotes(line)
      else:
         #print 'NOMATCH in SECTION'
         pass

   elif state == 'NOTES':
      if line.strip() == '':
         #print 'BLANKLINE'
         continue
      elif notes.match(line): sp.startNotes(line)
      elif section.match(line): sp.startSection(line)
      elif course.match(line): sp.startCourse(line)
      else:
         #print 'NOMATCH in NOTES'
         pass

   else:
      raise ValueError, "Unexpected input block state: " + state

f.close()

f = open(testfile, 'rb')

docstartcount = 0
deptcount = 0
coursecount = 0
sectioncount = 0
othercount = 0

for line in f:
   if sp.docstart.match(line):
      docstartcount += 1
   elif sp.department.match(line):
      deptcount += 1
   elif sp.course.match(line):
      coursecount += 1
   elif sp.section.match(line):
      sectioncount += 1
   else:
      othercount += 1
      
print ''
print 'EXPECTED:'
print 'start lines: %s' % docstartcount
print 'department lines: %s' % deptcount
print 'course lines: %s' % coursecount
print 'section lines: %s' % sectioncount
print 'other lines: %s' % othercount
print ''
print 'TOTALS:'
print 'course lines: %s' % sp.coursecount
print 'section lines: %s' % sp.seccount