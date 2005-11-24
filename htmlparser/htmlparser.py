#!/usr/bin/python
"""

HTML Parser using a stack. The idea is to allow the program to strip attributes
that are deprecated. However I want to have an --eric-meyer option that keeps the
tags identified in his book on CSS. The other thing is I would like to play
around with stack operations and handling HTML. Maybe I should consider incoporating
TIDY like functionality using the DTD...

"""
import HTMLParser

class Stripper:
    htmlDoc = ''

    def __init__(self, htmlSource):
        htmlDoc = htmlSource

    def printDoc(self):
        #p = HTMLParser()
        #p.feed(htmlDoc)
        #print p.get_starttag_text()
        #p.close()
        print self.htmlDoc

class Stack:
    def __init__(self):
        self.items = []

    def push(self, item):
        self.items.append(item)

    def pop(self):
        return self.items.pop()

    def isEmpty(self):
        return len(self.items) == 0

def main():
    import sys
    try:
        fin = sys.argv[1]
    except IndexError:
        sys.stderr.write('USAGE: %s <url | file.htm>\n'% sys.argv[0])
        sys.exit(1)

    if fin.startswith('http://'):
        import urllib
        sock = urllib.urlopen(fin)
        htmlSource = sock.read()
        sock.close()

    else:
        htmlSource = open(fin, 'rb').read()

    s = Stripper(htmlSource);
    s.printDoc()

if __name__ == "__main__":
    try:
        main()
    except IOError:
        pass