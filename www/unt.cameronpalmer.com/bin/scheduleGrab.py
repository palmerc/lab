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
            print '%s' % filetext
            course = re.compile(r'^\s*([A-Z]{4})\s(\d{4})\s+([ \w]+)')
            for line in f:
                match = course.match(line)
                if match:
                    print '%s %s %s' % match.groups()
                
            f.close()
                
            
