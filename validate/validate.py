"""
Program to ottomagically validate all pages in a specific domain

Author: Cameron Palmer
"""

import re, sys, os
import mechanize, pullparser, urllib2, urlparse

baseurl = "http://dev.coba.unt.edu/mgmt"

visited = []
queue = [baseurl]

def validatedata(b):
    f = file("tempfile.html", "wb")
    r = b.response()
    f.write(r.read())
    f.close()
    
    if os.system("tidy -quiet -e -f errors.txt tempfile.html") == 0:
        # yay
        print "valid", b.geturl()
    else:
        # bad
        print "FAILS", b.geturl()
    
def report():
    pass

def main():
    b = mechanize.Browser()
    b.set_handle_robots(False)
    
    while len(queue):
        url = queue.pop()
        try:
            b.open(url)
        except TypeError:
            try:
                b.follow_link(url)
            except:
                print "MAJOR ERROR REPORTING FOR DUTY"
        except urllib2.URLError, e:
            print "ERROR", url, e
            continue
        except UnicodeEncodeError, e:
            print "UNICODE ERROR", url
        except Exception, e:
            print "Error parsing", url, e
            
        visited.append(b.geturl())
        #TODO: 404 check goes here.  The verb is "continue" to bypass stuff
        #if onsite(url):
        if not b.viewing_html():
            continue
        validatedata(b)
            
            #TODO: Grab links
        
        for l in b.links():
            testlink = urlparse.urljoin(l.base_url, l.url).split('#')[0]
            if not testlink.startswith(baseurl):
                continue
            if not (testlink in queue or testlink in visited):
                queue.append(testlink)
   
    # follow second link with element text matching regular expression
    #try:
    #    response = b.follow_link(text_regex=re.compile(r"Management"), nr=0)
    #except:
    #    sys.exit(1)
    
    #print r.geturl()
    #print r.info()
    
main()