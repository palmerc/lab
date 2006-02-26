##     Version 2.2
## 
##     misc.py 1.1 -- miscellaneous support functions required for Prescript
##     Copyright (C) 1996  Todd Reed
##  
##     This program is free software; you can redistribute it and/or modify
##     it under the terms of the GNU General Public License as published by
##     the Free Software Foundation; either version 2 of the License, or
##     (at your option) any later version.
##  
##     This program is distributed in the hope that it will be useful,
##     but WITHOUT ANY WARRANTY; without even the implied warranty of
##     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##     GNU General Public License for more details.
##  
##     You should have received a copy of the GNU General Public License
##     along with this program; if not, write to the Free Software
##     Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
## 

#Module to provide misc functions for the analysis program

import os, re, sys, string
from string import atof


# defEqualness:
#
# Used as the default equalness in the isEqual function to do a loose comparison between a and b.
#
# expressed as a decimal, the comparison will fail if the difference is greater than defEq %

defEqualness = 0.003831

def MakeFilename(sourceName, newExt):
   head, tail = os.path.split(sourceName)
   root, ext = os.path.splitext(tail)
   return root + newExt

def short(string):
   return string.rstrip()

def checkforBib(string):
   
   bibPattern = re.compile(r'^\[[A-Za-z0-9\+]+\]\s*[A-Z]')

   position=bibPattern.search(string)
   if ((position==0) or (position==1)):
      return "bib"
   else:
      return "normal"

def findLastChar(string):
   def checkForParen(char):
      return char in [")", "]", "}", "\"", "\'"]

   def checkForPunc(char):
      if char in [".", "!", "?"]:
         return "stop"
      elif char==":":
         return "colon"
      else:
         return "other"

   string=short(string)
   char=string[(len(string)-1)]
   if checkForParen(char)==1:
      char=string[(len(string)-2)]

   return checkForPunc(char)

def getNum(string):
   string=string[4:]
   list=[]
   i=0
   for p in range(5):
      value=""
      done=1
      while(done!=0):
         if (i <=len(string)-1) and (string[i]!=" "):
            value=value + string[i]
            i=i+1
         else:
            done=0
            number=atof(value)
            list.append(number)
            i=i+1
   return list

def getLineSpace(string):
   position=re.pattern("@!@", string)
   if position!=-1:
      values=getNum(string)
      return value[1]-value[0]
   else:
      return 0

def firstCharCase(string):
   if string[0]=="[":
      string=string[3:]
   startParaPattern = re.compile(r'^\s*[A-Z]')
   if startParaPattern.search(string) >=0:
      return "true"
   else:
      return "false"
   


def foo_assert( expr, gsymtab, lsymtab ):
   if not eval(expr, gsymtab, lsymtab):
      sys.stderr.write( "assertion failed:"+expr+"\n" )



# determines if the two arguments are sufficiently "equal".
# governed by module default variable defEqualness
def isEqual( a, b, Equalness=defEqualness ):
   # hack to handle b==0 case.  there is probably a better action
   # than this, but this will do
   # print "%s\t%s"%(type(a),type(b))
   if b == 0:   return a <= Equalness
   return abs((float(a)/float(b))-1) <= Equalness


# readline:
#
# Wrapper to raw_input (or whatever) to return either a whole line of text
# or None, indicating end of file.
lno = 0
def readline():
   global lno
   try:
      lno = lno + 1
      return raw_input()
   except EOFError:
      return None


# nextline:
#
# Get next significant (ie, not comment etc) line of ARFF file.
#
# Essentially, all this function does is return the next non-
# empty, comment-stripped line from the input (as per
# readline())
def nextline():
   i = ""    
   while i == "":
      i = readline()
      if i == None:
         return None
      i = string.strip(i)
      cmntcol = string.find(i,'%')
      if cmntcol != -1:
         i = i[:cmntcol] # strip comments
   return i


# qsplit:
#
# like string.split() only quotes protect spaces
# fixed, and should now work for strings without delimeter too
def qsplit(i,delim=' '):
   r"""
   >>> qsplit("a b c")
   ['a', 'b', 'c']
   >>> qsplit("a \"b c\" d")
   ['a', 'b c', 'd']
   >>> qsplit("  ")
   ['', '', '']
   >>> qsplit(" ")
   ['', '']
   >>> qsplit("")
   []
   """
   if not i: return []
   piece = re.compile(r'(|".+?"|[^"].*?)('+delim+'|$)')
   if not piece.match(i):
       raise Exception, "No delimeter found"
   r = []
   for m in piece.finditer(i):
      g = m.group(1)
      if g and g[0] == "\"":
         r.append(g[1:-1])
      else:
        r.append(g)
   #FIXME: this is because only non-empty nuggets cause a null event
   if len(r) >= 2 and not r[-2]:
       return r
   return r[:-1]

# msg:
#
# print the given message to the message device, ususally stderr
def msg(msg,cr='\n'):
   sys.stderr.write(msg+cr)

if __name__ == "__main__":
   from doctest import testmod
   testmod()
