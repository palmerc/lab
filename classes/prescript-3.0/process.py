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
import os, sys, re, misc, math, statfunctions, __main__
from string import split, atoi, atof, find, strip
from misc import msg


################################################################################
##
##  Operational constants
##


# MinConsecFreq
#
# This is the minimum number of lines that must appear consecutively before
# their margins may be considered for inclusion
MinConsecFreq = 3


# MarginMinFreq:
#
# This the minimum number of times that an x1 value must appear to be eligible 
# to be considered as the right margin.  This is designed to prevent titles and
# other extraneous junk from colouring the calculation of rtmargin.
MarginMinFreq = 3


# MaxColumns:
#
# The maximum number of columns that may appear on one page
MaxColumns = 3


# Equalness:
#
# How equal two lengths or linespaces (X and Y) must be
Xequalness = 0.005
Yequalness = 0.05    # Equalness used for comparing linespaces
edgemargin = .025    # Equalness used in left margin comparisons in Page.calcIndent


# YES,NO:
#
# symbols to use for boolean values in Line
YES = 1
NO  = 0


class Line:
   # A Line is a line of text, constructed from Fragments.  The y
   # coordinates of all Fragments are saved to determine the best
   # value for the baseline.

   # Line classifications to be assigned
   Header = 1
   Footer = 2
   PageNo = 3
   Plain = 7

   def __init__(self, x0, x1, y, string ):
      self.string = string
      self.type = Line.Plain

      self.x0=x0
      self.x1=x1
      self.y=y
      self.rtmargin = 0 # right hand margin of this line.
   
      self.length = self.x1-self.x0
      self.linespace = '?'
      self.diffModeLineSpace = '?'
      self.diffModeLength = '?'
      self.rtwhitespace = '?' # amount of whitespace between rt edge & rt margin
      self.rightedge = '?'
      self.localAverage = '?'
      self.numberLocalAverage = '?'
   
      self.LS_CL_eq_MOLS = NO
      self.lastpagelastline_eq_MOLEN = NO
      self.LN_PL_eq_MOLEN = NO
      self.LN_CL_eq_MOLEN = NO
      self.isIndented = NO
      self.LS_CL_missing = NO
      self.LS_CL_eq_AVLS = NO
      self.LS_CL_eq_LS_NL = NO
      self.LS_CL_exactly_AVLS = NO
      self.LS_CL_lt_AVLS = NO
      self.LS_CL_gt_AVLS = NO
      self.isColumnbreak = NO
      self.LS_NL_eq_MOLS = NO
      self.NL_AVLS_eq_1 = NO
      self.RTedge_PL_eq_RTmargin = NO
      self.RTedge_CL_eq_RTmargin = NO
      self.RTedge_lastpagelastline_eq_RTmargin = NO
      
      self.diffLS_AVLS = '?'
      self.lastindent = '?'
      self.nextindent = '?'
      self.handclass = '?'
      self.prediction = '?'
      self.algoCase = '?'

   def getNumberLocalAverage(self):
      return self.numberLocalAverage 
      
   def getLocalAverage(self):
      return self.localAverage
      
   def getlength(self):
      return self.length
      
   def getLS(self):
      return self.linespace
      
   def gety(self):
      return self.y
      
   def getx0(self):
      return self.x0
      
   def getx1(self):
      return self.x1
      
   def getRTmargin(self):
      return self.rtmargin
      
   def setRTmargin(self, rtMargins):
      for m in range(len(rtMargins)):
         if m == len(rtMargins)-1 or misc.isEqual(self.getx1(), rtMargins[m], Xequalness) or self.getx1() < rtMargins[m]:
            self.rtmargin = rtMargins[m]
            break
      self.rtwhitespace = round(self.rtmargin-self.x1,1)

   def setLS(self, lastline):
      self.linespace = self.y - lastline.y

   def setRightEdge(self,modertm):
      for r in modertm:
         if misc.isEqual(self.x1,r,Xequalness):
            self.rightedge = 1
            return
         if misc.isEqual(self.x1,self.rtmargin,Xequalness):
            self.rightedge = 2
         else:
            self.rightedge = 3
   
   def reversePolarity(self):
      if self.linespace != "?":
         self.linespace =- self.linespace
    
   def __str__(self):
      def boolToStr(val):
         if val:
            return 'yes'
         else:
            return 'no '

      output = (
         boolToStr(self.LS_CL_eq_MOLS), \
         boolToStr(self.lastpagelastline_eq_MOLEN), \
         boolToStr(self.LN_PL_eq_MOLEN), \
         boolToStr(self.LN_CL_eq_MOLEN), \
         boolToStr(self.isIndented), \
         boolToStr(self.LS_CL_missing), \
         boolToStr(self.LS_CL_eq_AVLS), \
         boolToStr(self.LS_CL_eq_LS_NL), \
         boolToStr(self.LS_CL_exactly_AVLS), \
         boolToStr(self.LS_CL_lt_AVLS), \
         boolToStr(self.LS_CL_gt_AVLS), \
         boolToStr(self.isColumnbreak), \
         boolToStr(self.LS_NL_eq_MOLS), \
         boolToStr(self.NL_AVLS_eq_1),  \
         boolToStr(self.RTedge_PL_eq_RTmargin), \
         boolToStr(self.RTedge_CL_eq_RTmargin), \
         boolToStr(self.RTedge_lastpagelastline_eq_RTmargin), \
         self.linespace, \
         ## self.y, \
         ## self.length, \
         ## self.diffLS_AVLS, \
         ## self.diffModeLineSpace, \
         ## self.rightedge, \
         self.localAverage, \
         self.numberLocalAverage, \
         ## self.lastindent, \
         ## self.nextindent, \
         ## self.diffModeLength, \
         ## self.rtwhitespace, \
         self.algoCase, \
         self.prediction, \
         self.handclass)

      return ("%s,"*len(output))[:-1] % output




class Page:
   # Initialise regular expression's used for various things, here once and only once
   pageNoPattern = re.compile(r'^\s*\([^0-9]*\)\s*\(<page>[0-9]+\)\s*\\1 *$')
   headerPatternA = re.compile(r'^\s*\(<page>[0-9]+\)\s+CHAPTER\s+[0-9]+')
   headerPatternB = re.compile(r'^\s*[0-9]+\.\([0-9]+\.\)*\s+\([A-Za-z]+\)+\s*\(<page>[0-9]+\)\s*$')
   footerPattern = headerPatternB
   hyphenHeadPattern = re.compile(r'[a-z]-$')
   hyphenTailPattern = re.compile(r'^\([^\s]+\)[\s]*')

   def __init__(self):
      self.lines = [] #This array contains lines within each page
      self.all_ls = []
      self.all_len = []

   def __len__(self):
      return len(self.lines)
      
   def __getitem__(self,key):
      return self.lines[key]

   def __setitem__(self,key,val):
      self.lines[key]=val
      
   def __getslice__(self,i,j):
      return self.lines[i,j]

   def addLine(self, line):
      if len(self.lines) > 0:
         line.setLS( self.lines[-1] ) # set LS
         ls = line.getLS()
         self.all_ls.append( ls )
      self.all_len.append( line.getlength() )
      self.lines.append(line)

   def doneAddingLines(self):
      self.markPageNumbers()
      # Line numbers don't get predicted, so if a line number appears on the
      # first line, it must be 'removed' so that the following line appears
      # to be the first line.
      if len(self.lines)>=2 and self.lines[0].type != Line.Plain and self.lines[1].type == Line.Plain:
         self.lines[1].linespace = '?'

   def reversePolarity(self):
      self.all_ls = []  # this list is now invalidated
      for line in self.lines:
         line.reversePolarity()
         self.all_ls.append(line.getLS())
    
   def pageLength(self):
      return len(self.lines)

   def calcIndent(self, i, MOLS):
      global Yequalness, edgemargin

      currentEdge=self.lines[i].getx0()
      if i != 0:
         prevEdge=self.lines[i-1].getx0()
      else:
         prevEdge=currentEdge
         
      if i == len(self.lines)-1:
         nextEdge = prevEdge# if last line of page
      else:
         nextEdge = self.lines[i+1].getx0()

      # the following four lines adds a bias to each edge value if any of them are negative
      edges = [prevEdge,currentEdge,nextEdge]
      edges.sort()
      
      if edges[0] < 0:
         bias = abs(edges[0])
         prevEdge = prevEdge + bias
         currentEdge = prevEdge + bias
         nextEdge = prevEdge + bias

      # if the adjacent two lines are aligned *AND* the current line is indented
      if (self.lines[i].getLS() == "?" \
         or misc.isEqual(prevEdge,nextEdge,Xequalness) \
         or self.lines[i].getLS() > MOLS*10) \
         and currentEdge > ((1+edgemargin)*nextEdge):
         return abs(nextEdge-currentEdge) # Then the line is indented
      else:
         return 0 # otherwise not

   def calcLocalAverage( self, i, MOLS ):
      sum,n = self._do_calcLocalAverage( i, MOLS )
      self.lines[i].numberLocalAverage = n

      if (n !=0):
         return math.ceil(round(float(sum)/n,1))
      else:
         return self.lines[i].getLS()     # last resort

   def _do_calcLocalAverage(self, i, MOLS):
      # calcav:
      #
      # calculate the linespace average over the range of lines r.
      #  + sum and n may be initialised by optional arguments
      #  + noskip is a list of line numbers where calculation should
      #    not stop when a ? encountered.
      #  + Calculation will stop under the following conditions:
      #      - on contact with an LS = MOLS
      #      - on contact with a column break (approx 10*MOLS)
      #      - on contact with an LS = '?' (missing value)
      #      - End of lines listed in r
      #    MOLS will be included in the calculation, but no other
      #    values of LS which cause a stop will.
      
      def calcav( r, lines, MOLS, noskip, sum,n ):
         for index in r:
            linespace=lines[index].getLS()
   
            # check for stop condition
            if linespace == "?" or abs(linespace) > abs(10 * MOLS) or lines[index].type != Line.Plain:  
               if index in noskip:
                  continue
               else:
                  break
   
            sum=sum+linespace
            n=n+1
   
            # This stop condition must be checked afterwards
            if linespace == MOLS:
               break
         return sum,n
   
      # This is the beginning of AVLS version 1, which includes up to 5 lines
      # starting from i
      startidx = i
      endidx = startidx+5
      size = len(self.lines)
      
      if endidx > size:
         endidx = size  # bounds checking
   
      sum,n = calcav( range(startidx,endidx), self.lines, MOLS, [], 0, 0)  # make calculation
      if n == 1:
         # We would like a better idea of what AVLS is than just LS(CL).
         # Now we try to get an idea of AVLS by looking backward.
       
         # this constitutes AVLS version 2, which includes the previous two lines.
         # (AVLS2 actually includes more after i, but we already know there aren't any)
         startidx = i-2
         if startidx < 0:
            startidx = 0
         sum,n = calcav( range(i-1,startidx-1,-1), self.lines, MOLS, [], sum, n )
   
      return sum,n

   # set the remaining attributes 
   def calcValues(self, MOLS, MOLEN, modeRightMargins, lastPageLastLine):
      global Xequalness
      global Yequalness

      for i in range(len(self.lines)):
         self.lines[i].setRightEdge(modeRightMargins)           
   
         AVLS_CL=self.lines[i].localAverage=self.calcLocalAverage(i,MOLS)
         NL_AVLS=self.lines[i].getNumberLocalAverage() # NL_AVLS -- Number of lines in the local average calc
         LS_CL=self.lines[i].getLS() # LS_CL -- linespace of the current line
         if i < len(self.lines)-1:
            LS_NL=self.lines[i+1].getLS()
         else:
            LS_NL=10*MOLS
   
         if LS_CL == '?':
            self.lines[i].LS_CL_missing = YES
         
            if lastPageLastLine != None:
               if misc.isEqual(lastPageLastLine.getlength(),MOLEN,Xequalness): self.lines[i].lastpagelastline_eq_MOLEN = YES
               if misc.isEqual(lastPageLastLine.getx1(),lastPageLastLine.getRTmargin(),Xequalness): self.lines[i].RTedge_lastpagelastline_eq_RTmargin = YES
             
         else:
            # We are only interested in the absolute space between lines,
            # and no longer direction, and it is important that we make sure,
            # we do not have any negative values in the linespaces from now on.
            LS_CL=abs(LS_CL)
            LN_PL=self.lines[i-1].getlength() # LN_PL -- Length of the previous line
            LN_CL=self.lines[i].getlength() # LN_CL -- Length of the previous line
   
            self.lines[i].diffLS_AVLS = LS_CL-AVLS_CL
            self.lines[i].diffModeLineSpace = LS_CL-MOLS
            self.lines[i].diffModeLength = LN_CL-MOLEN
         
            if self.calcIndent(i, MOLS) != 0:
               self.lines[i].isIndented = YES
            if i > 0:
               self.lines[i].lastindent = abs(self.lines[i].getx0()-self.lines[i-1].getx0())
            if i < len(self.lines)-1:
               self.lines[i].nextindent = abs(self.lines[i].getx0()-self.lines[i+1].getx0())
   
            if misc.isEqual(LS_CL,MOLS,Yequalness):
               self.lines[i].LS_CL_eq_MOLS = YES
            if misc.isEqual(LS_CL,AVLS_CL,Yequalness):
               self.lines[i].LS_CL_eq_AVLS = YES
            if misc.isEqual(LS_CL,LS_NL,Yequalness):
               self.lines[i].LS_CL_eq_LS_NL = YES
            if LS_CL == AVLS_CL:
               self.lines[i].LS_CL_exactly_AVLS = YES
            if LS_CL < AVLS_CL:
               self.lines[i].LS_CL_lt_AVLS = YES
            if LS_CL > AVLS_CL:
               self.lines[i].LS_CL_gt_AVLS = YES
            if LS_CL > MOLS * 10:
               self.lines[i].isColumnbreak = YES
            if NL_AVLS == 1:
               self.lines[i].NL_AVLS_eq_1 = YES
            if misc.isEqual(LS_NL,MOLS,Yequalness):
               self.lines[i].LS_NL_eq_MOLS = YES
            if misc.isEqual(LN_PL,MOLEN,Xequalness):
               self.lines[i].LN_PL_eq_MOLEN = YES
            if misc.isEqual(LN_CL,MOLEN,Xequalness):
               self.lines[i].LN_CL_eq_MOLEN = YES
            if misc.isEqual(self.lines[i-1].getx1(),self.lines[i-1].getRTmargin(),Xequalness):
               self.lines[i].RTedge_PL_eq_RTmargin = YES
            if misc.isEqual(self.lines[i  ].getx1(),self.lines[i  ].getRTmargin(),Xequalness):
               self.lines[i].RTedge_CL_eq_RTmargin = YES


   # markPageNumbers determines whether the first and last lines of the page contain page numbers,
   # marking them so as appropriate
   def markPageNumbers(self):
      lines = self.lines

      if len(lines)<1:
         return # abort if no lines to work with

      for line in [lines[0],lines[-1]]: # both first and last lines
         if self.pageNoPattern.search(line.string) >= 0:
            line.type = Line.PageNo
            rawpn = self.pageNoPattern.group('page')
            
            if 0 < len( rawpn ) <=10:
               line.pageNo = atoi(rawpn)
            else:
               line.pageNo = -1

      line = lines[0] # first line
      
      if self.headerPatternA.search(line.string) >= 0:
         line.type = Line.Header
         rawpn = self.headerPatternA.group('page')
         if 0 < len( rawpn ) <=10:
            line.pageNo = atoi(rawpn)
         else:
            line.pageNo = -1
   
      if self.headerPatternB.search(line.string) >= 0:
         line.type = Line.Header
         rawpn = self.headerPatternB.group('page')
         if 0 < len( rawpn ) <=10:
            line.pageNo = atoi(rawpn)
         else:
            line.pageNo = -1
      
      line = lines[-1]  # last line
      
      if self.footerPattern.search(line.string) >= 0:
         line.type = Line.Footer
         rawpn = self.footerPattern.group('page')         
         if 0 < len( rawpn ) <=10:
            line.pageNo = atoi(rawpn)
         else:
            line.pageNo = -1

   def getRTmargins(self):
      return self.rtMargins
    
   # calcMargins:
   #
   #    This method establishes the number of columns and the margins of these
   # columns on the page.  A list of these margins is placed in self.rtMargins
   def calcMargins(self, MOLS, MOLEN):
      # generate a list of x1,y1 pairs.  From this a frequency graph can be
      # generated, and from that, the number of columns can be established
      x1list = []
      minLineLen = MOLEN/MaxColumns
      for line in self.lines:
         # only consider this line if it is wide enough to possibly be a whole line,
         # using the approximation that there will be at most, MaxColumns on one page.
         if line.getlength() > minLineLen or misc.isEqual(line.getlength(), minLineLen,Xequalness):
            x1list.append( (line.getx1(),line.gety()) )
      
      ## x{0,1}list contain (x,y) pairs for each line on the page ##

      # compress each list down to frequency,value pairs
      self.rtMargins = self.findFreq( x1list )
      self.rtMargins.sort( lambda a,b: int(a[1]-b[1]) )         # just to be quite sure

      ## {rt,lt}Margins NOW contain (f,x-av,y-av,y-min,y-max)   ##

      # We assume there will be no more than Maxcolumns (real) columns in the page.
      # More can be apparent due to tables, non-text objects, etc, and this
      # serves to eliminate some of these.
      if len(self.rtMargins) > MaxColumns:
         del self.rtMargins[:-MaxColumns]       
   
      if len(self.rtMargins) == 0:
         return # no columns were found, so can't apply them
      # lines with no right margin set will be handled in Page.predict_para
   
      # all the extra information is there for historical reasons
      # we need a copy of the margins to give to setRTmargin that doesn't have all that in it
      tmpRTmargins = []
      for m in self.rtMargins:
         tmpRTmargins.append(m[1])
   
      # now each line must be set with its margin as appropriate
      for l in self.lines:
         l.setRTmargin( tmpRTmargins )


   # findFreq:
   #
   # Finds the frequencies of each of the elements in the given list.
   # Note, this method re-arranges the order list.
   #
   # This particular version of findFreq has been customised in the following
   # ways:
   #  - the input is expected to be a list of tuples of the form:
   #    (x, y)
   #  - the output is a list of tuples of the form:
   #    (freq, x-av, y-av, y-min, y-max)
   #  - comparisons are done loosely, ie two adjacent numbers +/- 2 are
   #     considered to be "equal"
   #  - the x and y co-ordinates returned in the output list are averages
   #  - findFreq also filters out values with CONSECUTIVE frequencies below
   #    a certain limit.  This is supposed to help eliminate noise due to
   #    pictures etc.

   def findFreq( self, list ):
      if len(list) == 0:        # can't operate on nothing
         msg( "findFreq: WARNING: called with an empty input list!!" )
         return []

      output = []                       # output list
      f = 0                     # frequency of val
      val = list[0]             # start with the first element
      valcum = (0,0)            # cumulation of vals
      min = max = val[1]                # minimum and maximum y values
   
      # since val's aren't strictly the same, we'll average the val's to get a
      # more accurate view of what val actually is
   
      for l in list:
         if misc.isEqual(val[0],l[0],Xequalness):
            f = f + 1           # we've found another val
            valcum = (valcum[0] + l[0], valcum[1] + l[1])
            if l[1] < min:
               min = l[1]
            elif l[1] > max:
               max = l[1]
         else:
            output.append( (f, round(valcum[0]/f,1),round(valcum[1]/f,1), min, max ) )
            # reset
            val = l             # next val
            f = 1               # and we've already found one
            valcum = l
            min = max = val[1]

      output.append( (f, round(valcum[0]/f,1),round(valcum[1]/f,1), min, max) )
      output.sort()       # sort on frequency (pri) and x (sec) 

      # output now contains the list of consecutive frequencies.
   
      # I found the following few lines of code very difficult to comment,
      # so please excuse the bad explanation.
      #
      # 'output contains a list of (consecutive frequency,value) pairs.  That
      # means you can have several records that have the same value, but since
      # they weren't consecutive in the input list, they were not combined.
      #
      # for example, [(1,10),(2,8),(1,10),(1,9),(5,10)]
      #
      # in that example, 10 is the value with a sufficiently high frequency,
      # and so all values less than 10 must be deleted and all (x,10) records
      # must be combined.
   
      # this code finds the first record whose f is >= MinConsecFreq
      i = 0               # where to cut
      while i < len(output) and output[i][0] < MinConsecFreq:
         i = i + 1      # find first i where output[i] >= MinConsecFreq

      # if i points off the list, no eligible candidates were found.
      # When this happens, we take the largest frequency found, which will
      # be the last record in the list.
      if i == len(output):
         i = i - 1 

      # this code backtracks to ensure that any occurrances of the val
      # associated with the i'th record also gets included.  Without this,
      # the frequency reported for the val[i] might be too small because
      # some small groups (less than MinConsecFreq) were discounted
      # because their f's were too small.  Sort of.  Try and make sense
      # of that.
      j = 0
      while j < i:
         if misc.isEqual(output[j][1],output[i][1],Xequalness):
            j = j + 1
         else:
            del output[j]
         i = i - 1

      # output now contains only eligible frequencies which must now be combined
      # ie, [(1,10),(2,10),(1,11)] ==> [(3,10),(1,11)]
      #
      output.sort( lambda a,b: int(a[1] - b[1]) ) # sort on x value 
      i = 0
      while i <= len(output) - 2:       # go from 0 to second to last index
         if misc.isEqual( output[i][1], output[i+1][1],Xequalness ): #abs(output[i][1] - output[i+1][1]) <= 2:
            output[i] = self.mergeRecords( output[i], output[i+1], lambda a,b: round((a+b)/2) )
            del output[i+1]
         else:
            i = i + 1

      return output

   # mergeRecords
   #
   # take two tuples and merge them together
   #
   # Input is a list of tuples of the form:
   #    (f, x, y-av, y-min, y-max)

   def mergeRecords( self, k, j, minmax ):
      kf, kx, kav, kmin, kmax = k
      jf, jx, jav, jmin, jmax = j
      return kf+jf, minmax(kx,jx), round(((kav*kf)+(jav*jf))/(kf+jf),1), min([kmin,jmin]), max([kmax,jmax])

   # getPageNum:
   #
   # Attempts to find a page number on the page.  If none found, None is returned.
   #
   # WARNING: getPageNum does not attempt to check the 'sanity' (ie, not
   # greater than the total page count) of the fetched page number.  This may
   # be a possible bug.  The reason for omitting this test is that the number
   # of pages is not known at this stage, and to use that, a another pass
   # would be required.  I haven't yet considered which is the best approach
   # to take.  Sanity checking should be done later.
   def getPageNum(self):
      if len(self.lines) == 0:
         return None

      top_no = bot_no = None

      # find top and bottom page numbers
      tmp = [Line.PageNo, Line.Header, Line.Footer]

      if self.lines[0].type in tmp:
         top_no = self.lines[0].pageNo
      if self.lines[-1].type in tmp:
         bot_no = self.lines[-1].pageNo

      # if one (but not both) proposed page numbers exists, return it

      if top_no and not bot_no:
         return top_no
      elif not top_no and bot_no:
         return bot_no
      else:
         return None

   # dehyphenateLine:
   #
   # joins words that have been 'hyphenated'.  This is where a word is split
   # across two lines because the line was too long.  A line should be
   # dehyphenated when:
   #   - the linebreak is a linefeed
   #   - the previous line matches '[a-z]-$'
   #
   # On dehyphenation:
   #   - the previous line has trailing whitespace and hyphens removed
   #   - the previous line has the first 'word' (which is really the second
   #     half of the previous word) concatenated to the previous line.
   #
   # This means the previous line gets longer and the current line gets
   # shorter.  IT IS IMPORTANT TO NOTE, this method must be called AFTER
   # the linebreak has been predicted because it must only operate on 
   # linefeeds.  Also, the act of this method does not change the line
   # geometry, (ie, the length does not change.)
   #
   # Parameters:
   #   i        Line number to dehyphenate (Int)
   #    lpll    Reference to the last line of the last page (Line)
   def dehyphenateLine(self, i, lpll):
      if __main__.format == 'arff':
         return
      if i == 0:
         pl = lpll
      else:
         pl = self.lines[i-1]
      if pl == 'None':
         return #nothing to do
      cl = self.lines[i]

      if cl.prediction in ['linefeed','pagebreaklinefeed'] \
         and self.hyphenHeadPattern.search(pl.string) >= 0 \
         and self.hyphenTailPattern.search(cl.string) >= 0:
         pl.string = pl.string[:-1]+self.hyphenTailPattern.group(1)
         cl.string = cl.strip().sub(self.hyphenTailPattern, '', cl.string)

      # delete blank lines
      if cl.string == '':
         del self.lines[i]
         return 1
      return 0


class Document:
   def __init__(self):
      self.pages=[] #Contains the array of pages that comprise each document
      self.all_ls=[]
      self.all_len=[]
      self.modertmargins = []
      self.pagenums = []
      self.wasReversed = 0

   def __len__(self):
      return len(self.pages)
      
   def __getitem__(self,key):
      return self.pages[key]
      
   def __getslice__(self,i,j):
      return self.pages[i,j]

   # Sometimes the sign of the linespacing is reversed - This method checks
   # the sign of the majority of linespaces to determine wether they are 
   # reversed or not
   def checkPolarity(self):
      positive=0
      for ls in self.all_ls:
         if ls > 0:
            positive=positive+1

      # if less than half of all LS values are +ve, then the origin was flipped
      # and all LS values need negating
      if positive < 0.5*len(self.all_ls):
         self.reversePolarity()

   def reversePolarity(self):
      self.all_ls = []
      for page in self.pages:
         page.reversePolarity()
         self.all_ls = self.all_ls+page.all_ls

   def addPage(self, page):
      if page.pageLength()==0:
         return  # don't add empty pages

      pn = page.getPageNum()
      if pn != None:
         self.pagenums.append(pn)
      self.all_ls = self.all_ls+page.all_ls
      self.all_len = self.all_len+page.all_len
      self.pages.append(page)

   def doneAddingPages(self):
      # Makes a decision about page order.  Returns true if the sequence of
      # page numbers in PAGENUMS looks like a decreasing sequence.
      def pagesReversed(totalpages, pagenums):
         # If we don't have page numbers for most pages, don't force a
         # decision
         if len(pagenums)<.8*totalpages:
            return 0
   
         # Count the positive and negative changes in page numbers
         dpos = dneg = 0 
         for i in range(len(pagenums)-1):
            if pagenums[i]>totalpages:
               continue # sanity check
            delta = pagenums[i+1]-pagenums[i]
            if delta > 0:
               dpos=dpos+1
            elif delta < 0:
               dneg=dneg+1
   
         # reverse pages only if there are twice as many negative going as
         # there are positive going
         return dneg > 2*dpos
   
      if pagesReversed(len(self.pages), self.pagenums):
         self.wasReversed = 1
         self.pages.reverse()

   def calcMargins(self):
      # calculate column margins
      for page in self.pages:
         page.calcMargins(self.MOLS,self.MOLEN)
         rtm = page.getRTmargins()      # margins for this page

         for r in range(len(rtm)):
            if len(self.modertmargins) <= r:
               self.modertmargins.append([]) # ensure there's a list available
   
            # get the x value of the rth column and put it in the (document-wide) column-r list
            self.modertmargins[r].append(rtm[r][1])

      # find the mode margin for each column, ie reduce the collection of right margin X
      # values down to the most common X value.
      for r in range(len(self.modertmargins)):
         self.modertmargins[r] = statfunctions.Mode(self.modertmargins[r])

   def preprocess(self):
      self.checkPolarity()
      
      # calculate document wide mode values
      self.MOLS=statfunctions.Mode(self.all_ls)
      self.MOLEN=statfunctions.Mode(self.all_len)
   
      msg( '\tMOLS='+`self.MOLS` )
      msg( '\tMOLEN='+`self.MOLEN` )
   
      self.calcMargins()
      
      lastPageLastLine = None
      for i in range(len(self.pages)):
         self.pages[i].calcValues(self.MOLS,self.MOLEN,self.modertmargins, lastPageLastLine)
         if self.pages[i][-1].type == Line.Plain or len(self.pages[i]) < 2:
            n = -1
         else:
            n = -2
         lastPageLastLine = self.pages[i].lines[n]



###########################################################################
###########################################################################
###########################################################################

###########################################################################
###########################################################################
###########################################################################


class LineAssembler:
   def __init__(self, charWidth, pageAssembler):
      self.fragment = None
      self.yList = []

      self.charWidth = charWidth
      self.pa = pageAssembler

   # submit a fragment for line assembly
   def submit(self, fragment):
      if fragment == 'P':
         self._finishLine()
         self.pa.finishPage()

      elif self.fragment is None:
         self.fragment = fragment
         # self._addY(fragment)

      elif ((self.fragment.x1-fragment.x0) > (2*self.charWidth)) \
         and (fragment.y0 != self.fragment.y1) \
         or (abs(fragment.y0-self.fragment.y1) > (2*self.charWidth)):
         # this (new) fragment starts a new line
         self._finishLine()
         self.fragment = fragment
         # self._addY(fragment)

      else:
         # this (new) fragment continues this line
         self.fragment.concat(fragment)
         # self._addY(fragment)

   def _addY(self, fragment):
      self.yList.append(fragment.y0)
      self.yList.append(fragment.y1)

   def _finishLine(self):
      # Translate some characters that are known ligatures (mostly for TeX sources)
      def TranslateChars(string):
         string = string.replace('\013', 'ff')
         string = string.replace('\014', 'fi')
         string = string.replace('\015', 'fl')
         string = string.replace('\016', 'ffi')
         string = string.replace('\017', 'ffl')
         string = string.replace('\024', '<=')
         string = string.replace('\025', '>=')
         string = string.replace('\027A', 'AA')
         string = string.replace('\027a', 'aa')
         string = string.replace('\031', 'ss')
         string = string.replace('\032', 'ae')
         string = string.replace('\033', 'oe')
         string = string.replace('\034', 'o')
         string = string.replace('\035', 'AE')
         string = string.replace('\036', 'OE')
         string = string.replace('\037', 'O')
         string = string.replace('\256', 'fi')

         string = string.replace('\257', 'fl')
         string = string.replace('\366', 'fi')
         string = string.replace('\377', 'fl')
         string = string.replace('[\000-\037]', '?')
         string = string.replace('[\177-\377]', '?')
         return string

      if not self.fragment:
         return

      if type(self.fragment)==type(''):
         print 'self.fragment='+`self.fragment`
      self.fragment.string = TranslateChars(self.fragment.string)

      # self.yList.sort()
      # y = self.yList[len(self.yList)/2]

      self.pa.submit( Line( self.fragment.x0, self.fragment.x1, self.fragment.y0, self.fragment.string ) )

      self.yList=[]
      self.fragment = None

   # Must be called when no more fragments are left
   def done(self):
      self._finishLine()
      self.pa.done()


#
# The second pass worker class -- assembles each line from consecutive fragments
#
# The fragments submitted must be in order, that is, each fragment of the line
# must follow each other as they appear on the printed text.  If they are not,
# then the result will be that each non-consecutive fragment will begin a new
# line, when it shouldn't.
#
# The public methods of this class are:
#
#       newPage()       Called to create a new page
#       textFragment()  Submit next fragment
#       done()          Must be called after no more fragments are left
#
# The class is intialised with a fresh page.
#


class PageAssembler:
   def __init__(self, docAssembler):
      self.page = Page()
      self.da = docAssembler

   def submit(self, line):
      self.page.addLine( line )

   def finishPage(self):
      self.page.doneAddingLines()
      self.da.submit( self.page )
      self.page = Page()

   def done(self):
      self.finishPage()
      self.da.done()


class DocAssembler:
   def __init__(self):
      self.doc = Document()

   def submit(self, page):
      self.doc.addPage( page )

   def done(self):
      self.doc.doneAddingPages()

   def getDocument(self):
      return self.doc



# Convert fragment data into a list of pages, where each page is a list of
# Lines.  Note that the only attributes set are:
#         x0, x1, y, length, type, and string
#

def assembleDocument(frags):
   msg( "Assembling document" )

   docasm = DocAssembler()
   pageasm = PageAssembler( docasm )
   lineasm = LineAssembler(frags.modeCharWidth(), pageasm )
    
   for fragment in frags.getFragData():
      lineasm.submit( fragment )

   lineasm.done()
    
   return docasm.getDocument()

def preprocessDocument(document):
   msg( "Preprocessing document" )

   document.preprocess()

