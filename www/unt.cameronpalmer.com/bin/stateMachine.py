def startDoc():
    pass

def startDept():
    pass

def startCourse():
    pass

def startSection():
    pass

def startNotes():
    pass

## Starting state
state = 'DOCSTART'

docstart = re.compile(r'^\s*Updated:\s+(?P<updatedate>\d{1,2}/\d{1,2}/\d{4})' \
                    r'\s+(?P<updatetime>\d{1,2}:\d{1,2}:\d{1,2}AM|PM)' \
                    r'\s+Term:(?P<term>\d{4})' \
                    r'\s+(?P<session>Spring|Summer|Fall)\s+(?P<year>\d+)$')
department = re.compile(r'(?P<coursedept>[A-Z]{3,4})/(?P<extddept>[\w ]+)')
course = re.compile(r'^\s*(?P<coursedept>[A-Z]{3,4})\s(?P<coursenumber>\d{4})\s\s+(?P<coursetitle>.{1,30})\s*')
section = re.compile(r'^\s*(?P<section>\d{3})\s+\((?P<regcode>\d+)\)' \
                    r'\s+(?P<type>CRE|LAB|REC)\s+(?P<credits>[V\d.]+)\s*')
days = re.compile(r'\s(?P<days>[MTWRFSU]+)\s')
times = re.compile(r'\s(?P<starttime>\d{2}:\d{2}\s(?:am|pm))' \
                    r'-(?P<endtime>\d{2}:\d{2}\s(?:am|pm))\s')
classroom = re.compile(r'\s(?P<classroom>INET|(?:[A-Z]+\s[0-9]+))\s')
instructor = re.compile(r'\s\s+(?P<instructor>[A-Z]{0,1}[a-z-\']*\s*[A-Z]{0,1}[a-z-\']*\s*[A-Z]{0,1}[a-z-\']*)\s*$')

for line in inputtext:
    ## Hello Mister State Machine
    if state == "DOCSTART":
        if docstart.match(line): startDoc(line)
        if 
    
    elif state == "DEPARTMENT":
        if department.match(line): startDepart(line)

    elif state == "COURSE":
        if course.match(line): startCourse(line)

    elif state == "SECTION":
        if section.match(line): startSection(line)

    elif state == "NOTES":
        if notes.match(line): startNotes(line)

    else:
        raise ValueError, "Unexpected input block state: " + state
