"""

Mr. Docstring Buddy.

"""

import re, urllib, urlparse, picklecache, urllib2


class InvalidLink(Exception):
    pass

    
class Linky:
    def __init__(self, url):
        import md5
        self.url = url
        self.hash = "".join("%02x" % ord(i) for i in md5.new(self.url).digest())
        self.data = None
        
    def ishtml(self):
        cache = picklecache.picklecache(self.hash + "_head")
        try:
            headers = cache.serve()
        except:
            #TODO: re-enable the exceptions, test with a site that doesn't exist
            u = urllib.urlopen(self.url)
            headers = u.info()
            self.get_page(u.read())
            
            cache.save(headers)
        
        return headers.get("Content-Type", "")=="text/html"
    
    def get_page(self, html=None):
        """
        Pretty sure there's a logic error here...
        """
        cache = picklecache.picklecache(self.hash + "_page")
        if html is not None:
            data = Pagey(self.url, html)
            cache.save(data)
            return data
            
        try:
            data = cache.serve()
        except:
            data = Pagey(self.url)
            cache.save(data)
        return data        
    
    def __repr__(self):
        return self.url

class Pagey:
    r_a = re.compile(r"<a(.+?)>", re.I + re.DOTALL)
    r_href = re.compile("href=([^ >]+)")
    def __init__(self, url, data=None):
        self.url = url
        self.data = data
        #
        # TODO: Check validation and get list o' links
        #
    def link_iter(self):
        
        links = self.r_a.finditer(self.data)
        if links is None:
            raise Exception, "No links in page?!"
        for link in links:
            href = self.r_href.search(link.group(1))
            if href:
                yield urlparse.urljoin(self.url, href.group(1).strip("\""))
    
    def link_list(self):
        return [i for i in self.link_iter()]
                
        
    def __repr__(self):
        return "<Page data for %s of length %d>" % (self.url, len(self.data))

if __name__ == "__main__":
    l = Linky("http://google.com/")
    print "Trying link", l
    try:
        if l.ishtml():
            p = l.get_page()
            print "  We got the page", p
            #
            # merge links with queues and whatnot
            #
            print p.link_list()
            
        else:
            print "  Not html"
    except InvalidLink:
        print "Bad link!", l
    
    #p = Pagey("http://example.com/")
    #print p.url #prints http://example.com/