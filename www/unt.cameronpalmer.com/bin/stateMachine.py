import os, re

class scheduleParser:
    beginblank = re.compile(r'^(\s+)')
    docstart = re.compile(r'^Updated:\s+(?P<updatedate>\d{1,2}/\d{1,2}/\d{4})' \
                    r'\s+(?P<updatetime>\d{1,2}:\d{1,2}:\d{1,2}AM|PM)' \
                    r'\s+Term:(?P<term>\d{4})' \
                    r'\s+(?P<session>Spring|Summer|Fall)\s+(?P<year>\d+)$')
    department = re.compile(r'^\s*(?P<coursedept>[A-Z]{3,4})/(?P<extddept>.+)$')
    course = re.compile(r'^\s*(?P<coursedept>[A-Z]{3,4})\s+(?P<coursenumber>\d{4})\s+(?P<coursetitle>.+)$')
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
        
    def startDoc(self, line):
        self.state = 'DOCSTART'

        docmatch = self.docstart.match(line)        
        self.updatedate = docmatch.group('updatedate')
        self.updatetime = docmatch.group('updatetime')
        self.term = docmatch.group('term')
        self.session = docmatch.group('session')
        self.year = docmatch.group('year')

    def startDepart(self, line):
        self.state = 'DEPARTMENT'
        self.deptlist = line.strip().split('/')
        (self.dept, self.deptlong) = self.deptlist

    def startCourse(self, line):
        self.state = 'COURSE'

        blankmatch = self.beginblank.match(line)
        if blankmatch:
            (start, end) = blankmatch.span()
            self.courseblankcount = end - start
            print self.courseblankcount

    def startSection(self, line):
        self.state = 'SECTION'

        blankmatch = self.beginblank.match(line)
        if blankmatch:
            (start, end) = blankmatch.span()
            self.secblankcount = end - start
            print self.secblankcount
        
    def startNotes(self, line):
        self.state = 'NOTES'
        
        blankmatch = self.beginblank.match(line)
        if blankmatch:
            (start, end) = blankmatch.span()
            self.secblankcount = end - start
            print self.secblankcount

testfile = '../data/txt/1061/mathematics_1061.txt'

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
    print sp.state
    line = line.rstrip()
    line = line.lstrip('\f')
    
    ## Hello Mister State Machine
    if sp.state == 'DOCSTART':
        if line.strip() == '': print 'BLANKLINE'
        elif sp.docstart.match(line): sp.startDoc(line)
        elif sp.department.match(line): sp.startDepart(line)
        else:
            #print 'NOMATCH in DOCSTART'
            pass
        
    elif sp.state == 'DEPARTMENT':
        if line.strip() == '': print 'BLANKLINE'
        elif sp.department.match(line): sp.startDepart(line)
        elif sp.course.match(line): sp.startCourse(line)
        else:
            #print 'NOMATCH in DEPARTMENT'
            pass

    elif sp.state == 'COURSE':
        if line.strip() == '': print 'BLANKLINE'
        elif sp.course.match(line): sp.startCourse(line)
        elif sp.section.match(line): sp.startSection(line)
        else:
            #print 'NOMATCH in COURSE'
            pass

    elif sp.state == 'SECTION':
        if line.strip() == '': print 'BLANKLINE'
        elif sp.section.match(line): sp.startSection(line)
        elif sp.course.match(line): sp.startCourse(line)
        #elif notes.match(line): startNotes(line)
        else:
            #print 'NOMATCH in SECTION'
            pass

    elif state == 'NOTES':
        if line.strip() == '': print 'BLANKLINE'
        elif notes.match(line): sp.startNotes(line)
        elif section.match(line): sp.startSection(line)
        elif course.match(line): sp.startCourse(line)
        else:
            #print 'NOMATCH in NOTES'
            pass

    else:
        raise ValueError, "Unexpected input block state: " + state
