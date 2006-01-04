#Download PDF class catalogs and convert to text

import urllib2, os, sys, re
sys.path.append("BeautifulSoup-2.1.1/")
from BeautifulSoup import BeautifulSoup

if __name__ == '__main__':
    """ Download the PDF class schedules convert them to text 
    and store them """
    
    base = 'http://essc.unt.edu/registrar/SOCbydept/'
    url = 'SOCbydeptA.htm'
    datadir = '../data/pdf/'
    html = urllib2.urlopen(base+url).read()
    soup = BeautifulSoup()
    soup.feed(html)
    for anchor in soup('a'):
        link = anchor['href']
        if link[-4:] == '.pdf':
            fileurl = base+link
            fileout = datadir+link.lower()
            fileout = fileout.replace('%20', '_')
            filetext = fileout.replace('.pdf', '.txt')
            directory = datadir+link.split('/', 1)[0]
           try:
                os.mkdir(directory)
            except:
                pass
            os.popen4('wget -q -O %s %s' % (fileout, fileurl))
            os.popen4('pdftotext -layout %s %s' % (fileout, filetext))
            f = open(filetext, "rb")
            print filetext
            course = re.compile(r'^\s*(?P<coursedept>[A-Z]{3,4})\s(?P<coursenumber>\d{4})\s+(?P<coursetitle>.+)')
            regsection = re.compile(r'^\s*(?P<section>\d{3})\s+\((?P<regcode>\d+)\)' \
                                 r'\s+(?P<type>CRE|LAB|REC)\s+(?P<credits>[\d.]+)' \
                                 r'\s+(?P<days>[MTWRFSU]+)\s+(?P<starttime>\d{2}:\d{2}\s*am|pm)' \
                                 r'-(?P<endtime>\d{2}:\d{2}\s*am|pm)\s+(?P<classroom>[A-Z]+\s+\d+)' \
                                 r'\s*(?P<instructor>[\w ]*)\s*$')
            specsection = re.compile(r'^\s*(?P<section>\d{3})\s+\((?P<regcode>\d+)\)' \
                                 r'\s+(?P<type>CRE|LAB|REC)\s+(?P<credits>V)')                

            for line in f:
                line = line.strip()
                coursematch = course.match(line)
                regsectionmatch = regsection.match(line)
                specsectionmatch = specsection.match(line)
                if coursematch:
                    print '%s %s %s' % coursematch.groups()
                if regsectionmatch:
                    print '%s %s %s %s %s %s %s %s %s' % regsectionmatch.groups()
                if specsectionmatch:
                    print '%s %s %s %s' % specsectionmatch.groups()
                    
                      
            f.close()
                
            
