##     Version 2.2
## 
##     Copyright (C) 1998  David Miller
##
##     Based in part on prescript 0.1 (c) 1996 Todd Reed
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
import __main__

import os, sys, misc, statfunctions, string
from process import Line
from string import split, atoi, find, strip
from urllib import unquote
from misc import msg


# knownFormats:
#
# Formats known by the io module.  May be used for checking externally.
knownFormats = ['txt','html','arff']

# maxBadLines:
#
# Error count limit for reading in PS data
maxBadLines = 6

# Class Fragment:
#
# This represents a single fragment of text read in from a PS file.  It is
# capable of generating basic statistics about the fragment.
class Fragment:

   # A Fragment is a unit of text and its start and end points

   def __init__(self, x0, y0, s, x1, y1):
      self.x0 = x0
      self.y0 = y0
      self.x1 = x1
      self.y1 = y1
      self.string = s

   def averageCharWidth(self):
      return int(((self.x1-self.x0)/float(len(self.string)))+0.5)

   def concat(self, fragment):
       # if this new fragment starts a new word, insert enough space to reflect this
       avgCharWidth = min([self.averageCharWidth(), fragment.averageCharWidth()])
       if fragment.x0-self.x1 > 0.3*avgCharWidth:
	   # append fragment's string component plus at most 10 spaces to preserve
	   # some nature of the spacing between fragments
	   if avgCharWidth == 0: wordspacing = 1
	   else: wordspacing=min(max(int((fragment.x0-self.x1)/avgCharWidth + 0.5), 1), 10)
	   self.string=self.string+' '*wordspacing

       self.x1 = fragment.x1
       self.y1 = fragment.y1
       self.string = self.string+fragment.string

   def __str__(self):
      return "("+`self.x0`+", "+`self.y0`+", "+self.string+")"



###########################################################################
###########################################################################
##
##   Input routines
##
###########################################################################
###########################################################################


# Obtains fragments from the given initialised file handle and applies the
# read fragments to the worker instance.
#
# The worker instance is the mechanism for storing and manipulating the
# fragments of text.  It must have the following methods:
#
#	    newPage()	      Start a new page
#	    textFragment()    Present new text fragment
#	    done()	      Document read, cleanup code
#
# The first page is assumed to have already been initialised.
def readPostScriptDataFile(FH, worker):
    errs = 0
    while 1:
	input = FH.readline()
	if not input: break

	input = strip(input)
	if len(input) == 0: continue

	input = split(input, '\t')
	if input[0][0] == "P":
	    worker.newPage()
	    
	elif input[0][0] == "S" and len(input) == 8:
	    [tag, x0, y0, string, ytop, ybot, x1, y1] = input
	    
	    # If x1 is 'S', then some funny recursive font stuff has happened.
	    # Ignore the recursive stuff, and search for the rest of this line
	    if x1 == "S":
		while 1:
		    input = FH.readline();
		    if input[0] != "S": break
		[x1, y1] = split(input[:-1], '\t')[:2]
		
	    string = unquote(string)  
	    if len(string) > 0:
		worker.textFragment( Fragment( atoi(x0), atoi(y0),
					       string,
					       atoi(x1), atoi(y1)))
	else:
	    msg( "Bad fragment line: "+`input`)
	    errs = errs + 1
	    if errs == maxBadLines:
		msg( 'Error limit encounter, aborting.  Is this *really* a post script file?' )
		sys.exit(1)
    worker.done()


# The first pass worker class -- reads PS data and gathers statistics about the
# text fragments.  Builds fragment data structure
class PSDatReader:
   def __init__(self):
       self.psdata = []
       self.charWidths = []  # all the charWidth values get stuffed in here

   def done(self):
       pass
   
   def newPage(self):
       self.psdata.append( 'P' )

   def textFragment(self, fragment):
       self.psdata.append( fragment )
       self.charWidths.append( fragment.averageCharWidth() )

   def modeCharWidth(self):
       return statfunctions.Mode(self.charWidths)

   def getFragData(self):
       return self.psdata



# Generates and reads line fragments from the given postscript filename.
#
# Returns an instance of PSDatReader, which contains the read fragments
# plus some extra statistics required by the next pass
def readFragments(psFilename):
    msg( "Reading PS fragments" )
    gspipe = os.popen("gs -q -dNODISPLAY -soutfile=%%stdout %sprescript.ps %s quit.ps" % (__main__.prescript_dir, psFilename))
    fragdata = PSDatReader()
    readPostScriptDataFile( gspipe, fragdata )
    return fragdata

    

    


   
 

###########################################################################
###########################################################################
##
##   Output routines
##
###########################################################################
###########################################################################

# Straight ASCII dump of document (minimal formatting)
class PlainTextFormatter:
    def __init__(self, out):      self.out = out
    def start(self, doc):	  pass
    def end(self, doc):		  pass
    
    def linefeed(self):		  self.out.write('\n')
    def paragraph(self):	  self.out.write('\n\n')
    def pagebreaklinefeed(self):  self.linefeed()
    def pagebreakparagraph(self): self.paragraph()
    def explicitlinefeed(self):   self.out.write('\n')
    def pagebreak(self):	  self.out.write('\n\n'+'-'*80+'\n')
    def line(self, l):		  self.out.write(l.string)

# Outputs compliant to the ARFF version 2 format to be used with ML appropriate
# schemes 
class ARFFFormatter:
    # ARFFtypes:
    #
    # This list specifies which types of lines should be included in ARFF output
    # This also is the list which specifies which types of lines have HANDCHECK
    # (ground truth) data
    ARFFtypes = [Line.Plain]
    
    def __init__(self, out):      self.out = out
    def start(self, doc):
	try:
	    ahdr = open( __main__.prescript_dir+'arff-header', 'r+' )
	    self.out.writelines( ahdr.readlines() )
	    ahdr.close()
	except IOError:
	    msg( 'WARNING: Header %s does not exist' % (__main__.prescript_dir+'arff-header') )
    
    def end(self, doc):		  pass
    
    def linefeed(self):		  pass
    def paragraph(self):	  pass
    def pagebreaklinefeed(self):  self.linefeed()
    def pagebreakparagraph(self): self.paragraph()
    def explicitlinefeed(self):   pass
    def pagebreak(self):	  pass
    def line(self, l):
	if l.type in ARFFFormatter.ARFFtypes: self.out.write(str(l)+'\n')

# Permits output in HTML format
class HTMLFormatter:
    def __init__(self, out):      self.out = out
    def start(self, doc):
	if doc.wasReversed: self.out.write( "<!--Pages Reversed-->\n" )
    def end(self, doc):		  pass
    
    def linefeed(self):		  self.out.write('\n')
    def paragraph(self):	  self.out.write('\n<p>')
    def pagebreaklinefeed(self):  self.linefeed()
    def pagebreakparagraph(self): self.paragraph()
    def explicitlinefeed(self):   self.out.write('<br>\n')
    def pagebreak(self):	  self.out.write('\n\n<!--End Of Page--><p><hr><p>\n')
    
    def line(self, l):
	if l.type == Line.Header or l.type == Line.Footer: self.out.write("<p><i><center>%s</center></i><p>" % self.HTMLQuote(l.string) )
	elif l.type == Line.PageNo:	                   self.out.write("<!--Page No--><p><b><center>%s</center></b><p>\n" % self.HTMLQuote(l.string) )
	else:						   self.out.write(self.HTMLQuote(l.string))

    def HTMLQuote(self,s):
	quote_chars = '<>&"'
	entities = ("&lt;", "&gt;", "&amp;", "&quot;")
	res = ''
	for c in s:
	    index = find(quote_chars, c)
	    if index >= 0:  res = res + entities[index]
	    else:	    res = res + c
	return res


# Render the provided Page instance with the given Formatter instance
def renderPage(formatter, page):
    for line in page:
	eval( "formatter."+line.prediction+"()" )
	formatter.line(line)
    formatter.pagebreak()

# Render the provided Document instance with the given Formatter instance
def renderDocument(formatter, document):
    msg( "Rendering document" )
    
    formatter.start(document)
    for page in document:   renderPage(formatter, page)
    formatter.end(document)

# Create an instance of the appropriate formatter 
def newFormatter(format, out):
    if format == 'txt': return PlainTextFormatter(out)
    if format == 'html': return HTMLFormatter(out)
    if format == 'arff': return ARFFFormatter(out)
    return None

# applyHandcheck:
#
# Consider this function highly experimental and of no interest to anyone but
# the NZDL.
#
# This function is only relevant to those who are using the output of prescript
# for ML experiments involving software produced by the University of Waikato
# Machine Learning group.  It detects the presence of a ground-truth (hand-check)
# file and applies it to all relevant lines in the document.
def applyHandcheck(inputFilename, document):
    hcstring={'0':'linefeed','1':'paragraph','2':'pagebreaklinefeed','3':'pagebreakparagraph','4':'explicitlinefeed','5':'picnoise'}
    handcheckFN = misc.MakeFilename(inputFilename,'.handclass')
    if os.path.exists( handcheckFN ):
	msg( 'Found and applying handcheck file' )
	hcf = open( handcheckFN )

	for page in document:
	    for line in page:
		try:
		    if line.type in ARFFFormatter.ARFFtypes:
			line.handclass = hcstring[string.strip(hcf.readline())]
		except KeyError, val:
		    if val in [None,'']:  # cope with EOF
			msg( 'ERROR: Handclass file ran out before the end of the document!' )
			return
		    # otherwise it's a bad keyword
		    msg( 'Unknown classification: '+`val` )

		    
