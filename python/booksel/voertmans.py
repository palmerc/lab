#!/usr/bin/python
"""

Access Voertmans' info online

"""

import urllib, re

class VoertmansSession:
    base = "http://www.voertmans.com/textbooks.asp"
    active = ""
    
    r_sid       = re.compile(r"textbooks\.asp\?mscssid=([a-z0-9]+)", re.IGNORECASE);
    r_sem_block = re.compile(r"<select name=campusterm_id.+?>(.+?)</select", re.DOTALL)
    r_sem       = re.compile(r"<OPTION VALUE=\"(.+?)\".*?>([^\r]+)")
    
    
    def __init__(self):
        self.active = self.base + "?mscssid=" + self.get_sid()
        
    def get_sid(self):
        p = picklecache("sid")
        try:
            return p.serve()
        except:
            print "--> fetching sid"
        x = readurl(self.base)
        sid = extract(x, self.r_sid)
        print "--> got a sessionid of ", sid
        p.save(sid)
        return sid
        
    def firstsemester(self):
        p = picklecache("semesters")
        try:
            block = p.serve()
        except:
            print "--> fetching first semester"
            x = readurl(self.active)
            block = extract(x, self.r_sem_block)
            p.save(block)
        for semester in self.r_sem.finditer(block):
            g = semester.groups()
            if g[0]=="0": continue
            return VoertmansSemester(self, *g)
        
    def fetch(self, args=None):
        return readurl(self.active, args)
        
    # replacing bare & with &amp;
    #     &(?!#?[xX]?(?:[0-9a-fA-F]+|\w{1,8});)

class VoertmansSemester:
    r_dept_block = re.compile(r"<select name=dept_id.+?>(.+?)</select", re.DOTALL)
    r_dept       = re.compile(r"<OPTION VALUE=\"(.+?)\".*?>([^\r]+)")
    
    def __init__(self, session, id, title):
        self.session=session
        self.campusterm_id=id
        (self.campus_id, self.term_id) = id.split("|")
        self.title=title
    
    def __repr__(self):
        return self.title
    
    def dept_dict(self):
        p = picklecache("depts")
        try:
            block = p.serve()
        except:
            x = self.session.fetch({"step": 1, "campus_id": self.campus_id, "term_id": self.term_id, "campusterm_id": self.campusterm_id})
            block = extract(x, self.r_dept_block)
            p.save(block)
        r={}
        for dept in self.r_dept.finditer(block):
            g = dept.groups()
            if g[0]=="0": continue
            (n,d) = g[1].split(" - ")
            r[n] = VoertmansDept(self, n, d, int(g[0]))
        return r


class VoertmansDept:
    r_course_block = re.compile(r"<select name=course_id.+?>(.+?)</select", re.DOTALL)
    r_course       = re.compile(r"<OPTION VALUE=\"(.+?)\".*?>([^\r]+)")

    def __init__(self, semester, name, title, id):
        self.id=id
        self.name=name
        self.title=title
        self.session=semester.session
        self.semester=semester

    def __repr__(self):
        return self.title

    def course_dict(self):
        p = picklecache("courses_"+self.name)
        try:
            block = p.serve()
        except:
            x=self.session.fetch({"campusterm_id": self.semester.campusterm_id, "campus_id": self.semester.campus_id, "term_id": self.semester.term_id, "dept_id": self.id, "step": 2})
            block = extract(x, self.r_course_block)
            p.save(block)
        r={}
        for course in self.r_course.finditer(block):
            g = course.groups()
            if g[0]=="0": continue
            r[int(g[1])] = VoertmansCourse(self, int(g[0]), int(g[1]))
        return r
        


class VoertmansCourse:
    def __init__(self, dept, id, number):
        self.session=dept.session
        self.semester=dept.semester
        self.dept=dept
        self.id=id
        self.dept=dept
        self.number=number
    def __repr__(self):
        return "%s %d" % (self.dept.name, self.number)
    def books(self):
        pass


class picklecache:
    import cPickle
    def __init__(self, fn):
        self.fn = "cache/" + fn
    def serve(self):
        f = file(self.fn, "rb")
        return self.cPickle.load(f)
    def save(self, stuff):
        f = file(self.fn, "wb")
        self.cPickle.dump(stuff, f)

    
def readurl(url, postvars=None):
    print "--> %s" % url
    
    if postvars is not None:
        postvars = urllib.urlencode(postvars)
        
    f = urllib.urlopen(url, postvars)
    data = f.read()
    f.close()
    return data

class RegexpException(Exception):
    pass

def extract(thing, regexp):
    result = regexp.search(thing)
    if not result:
        raise RegexpException, "Couldn't find needle in haystack"
    return result.groups()[0]

def main():
    v = VoertmansSession()
    d = v.firstsemester().dept_dict()
    print d["CSCE"].course_dict()
    
if __name__ == "__main__":
    main()