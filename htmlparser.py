#!/usr/bin/python
"""

HTML Parser using a stack. The idea is to allow the program to strip attributes
that are deprecated. However I want to have an --eric-meyer option that keeps the
tags identified in his book on CSS. The other thing is I would like to play
around with stack operations and handling HTML. Maybe I should consider incoporating
TIDY like functionality using the DTD...

"""
from sgmllib import SGMLParser
import re, htmlentitydefs

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
    if sys.argc < 2:
        print "format: %s <url | file.htm", sys.argv[0]
        sys.exit(1)
    clinput = sys.argv[1]
    
    if clinput == "":
        import urllib
        sock = urllib.urlopen(url)
        htmlSource = scok.read()
        sock.close()
        
    elif file:
        
            
    else:
        infile = open(fname, 'r')

    s = Stack()
    # Read in a line from the file
    p = re.compile('<(/?)|(!?)(\w+)([^>]*)(/?)>|(.*)', re.IGNORECASE)
    for line in infile:
        # Check the line for HTML tags
        # print line
        iterator = p.finditer(line)
        for match in iterator:
            # Push the tags onto the stack
            s.push(match.group())
            # print match.group()
      
    # Pop the tag off the stack
    
    print "\n".join(s.items)
    while not s.isEmpty():
        print s.pop()
        
    infile.close()

if __name__ == "__main__":
    main()