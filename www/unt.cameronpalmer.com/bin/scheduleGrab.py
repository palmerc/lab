#Download PDF class catalogs and convert to text

import urllib2, os
from BeautifulSoup import BeautifulSoup

if __name__ == '__main__':
    """ Download the PDF class schedules from UNT and store them """
    
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
