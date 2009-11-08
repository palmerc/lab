import urllib2,os
from BeautifulSoup import BeautifulSoup,NavigableString

specResults = 'http://www.spec.org/cpu2006/results/'
intMarks = 'cint2006.html'
dloadDirectory = '/Users/palmerc/specBenchmarkCSVs/'

if not os.access(dloadDirectory, os.F_OK):
    os.mkdir(dloadDirectory)

html = urllib2.urlopen(specResults + intMarks).read()
soup = BeautifulSoup(html)

tags = soup.findAll('a')

count = 0
for tag in tags:
    if tag.string == "CSV":
        relFileName = tag['href']
        url = specResults + relFileName
        fileName = relFileName.split('/')[-1]
        
        print "-", fileName
        fin = urllib2.urlopen(url)
        fout = open(dloadDirectory + fileName, 'w')
        
        fout.write(fin.read())
        fin.close()
        fout.close()
        count += 1
print
print "Downloaded a total of", count, "files."
