#Download PDF class catalogs and convert to text

__author__ = "Cameron Palmer"
__copyright__ = "Copyright 2005, Cameron Palmer"
__version__ = "$Rev$"
__license__ = "GPL"

import os, sys, re

if __name__ == '__main__':
    """Convert the PDF versions of the schedule to TXT files"""
    
    datadir = '../data/pdf/'
            fileurl = base+link
            fileout = datadir+link.lower()
            fileout = fileout.replace('%20', '_')
            filetext = fileout.replace('.pdf', '.txt')
            directory = datadir+link.split('/', 1)[0]
            try:
            	os.mkdir(directory)
            except:
            	print 'ERROR!'
            	pass
            	
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
                
            
