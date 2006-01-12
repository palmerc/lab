import os, re

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

def startDoc():
    state = 'DOCSTART'
    print state

def startDept():
    state = 'DEPARTMENT'
    print state

def startCourse():
    state = 'COURSE'
    print state

def startSection():
    state = 'SECTION'
    print state
    
def startNotes():
    state = 'NOTES'
    print state
    
global state

## Starting state
state = 'DOCSTART'
testfile = '../data/txt/1061/mathematics_1061.txt'

f = open(testfile, 'rb')

blankln = re.compile(r'^\s*\n*')
docstart = re.compile(r'^\s*Updated:\s+(?P<updatedate>\d{1,2}/\d{1,2}/\d{4})' \
                    r'\s+(?P<updatetime>\d{1,2}:\d{1,2}:\d{1,2}AM|PM)' \
                    r'\s+Term:(?P<term>\d{4})' \
                    r'\s+(?P<session>Spring|Summer|Fall)\s+(?P<year>\d+)$')
department = re.compile(r'(?P<coursedept>[A-Z]{3,4})/(?P<extddept>.+)')
course = re.compile(r'^\s*(?P<coursedept>[A-Z]{3,4})\s(?P<coursenumber>\d{4})\s\s+(?P<coursetitle>.{1,30})\s*')
section = re.compile(r'^\s*(?P<section>\d{3})\s+\((?P<regcode>\d+)\)' \
                    r'\s+(?P<type>CRE|LAB|REC)\s+(?P<credits>[V\d.]+)\s*')
days = re.compile(r'\s(?P<days>[MTWRFSU]+)\s')
times = re.compile(r'\s(?P<starttime>\d{2}:\d{2}\s(?:am|pm))' \
                    r'-(?P<endtime>\d{2}:\d{2}\s(?:am|pm))\s')
classroom = re.compile(r'\s(?P<classroom>INET|(?:[A-Z]+\s[0-9]+))\s')
instructor = re.compile(r'\s\s+(?P<instructor>[A-Z]{0,1}[a-z-\']*\s*[A-Z]{0,1}[a-z-\']*\s*[A-Z]{0,1}[a-z-\']*)\s*$')

for line in f:
    line = line.rstrip()
    line = line.lstrip('\f')
    print line
    
    ## Hello Mister State Machine
    if state == "DOCSTART":
        if blankln.match(line): blankLine=1
        elif docstart.match(line): startDoc(line)
        else:
            pass
    
    elif state == "DEPARTMENT":
        if department.match(line): startDepart(line)
        elif course.match(line): startCourse(line)
        else:
            pass

    elif state == "COURSE":
        if course.match(line): startCourse(line)
        elif section.match(line): startSection(line)
        else:
            pass

    elif state == "SECTION":
        if section.match(line): startSection(line)
        elif course.match(line): startCourse(line)
        elif notes.match(line): startNotes(line)
        else:
            pass

    elif state == "NOTES":
        if notes.match(line): startNotes(line)
        elif section.match(line): startSection(line)
        elif course.match(line): startCourse(line)

    else:
        raise ValueError, "Unexpected input block state: " + state
