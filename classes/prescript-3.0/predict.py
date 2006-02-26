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

import misc
from process import Line

def predict(l):
   # extract each datum into a convenient variable

   # The following three lines are a bit of a hack to make the algorithm work
   # better.  We will say that a line = mode length, when either of the
   # following  are true:
   # - length(x) == modeLineLength 
   # - x1 == rtmargin
   #
   # specifically, this hack means bullet-indents, etc will be considered
   # line feeds dispite their length not being equal to the mode length.
   LN_PL_eq_MOLEN = l.LN_PL_eq_MOLEN or l.RTedge_PL_eq_RTmargin
   LN_CL_eq_MOLEN = l.LN_CL_eq_MOLEN or l.RTedge_CL_eq_RTmargin
   lastpagelastline_eq_MOLEN = l.lastpagelastline_eq_MOLEN or l.RTedge_lastpagelastline_eq_RTmargin
   
   LS_CL_leq_AVLS = l.LS_CL_exactly_AVLS or l.LS_CL_lt_AVLS
   LS_CL_geq_AVLS = l.LS_CL_exactly_AVLS or l.LS_CL_gt_AVLS
    
   # start of algorithm, proper

   if l.LS_CL_missing:
      if lastpagelastline_eq_MOLEN:
         return '0','pagebreaklinefeed' #0
      else:
         return '0','pagebreakparagraph' #0

   if l.LS_CL_eq_MOLS and LN_PL_eq_MOLEN:
      if l.isIndented and l.LS_NL_eq_MOLS:
         return 'A','paragraph' #A
      else:
         return 'B','linefeed' #B

   if LN_PL_eq_MOLEN and not l.LS_CL_eq_MOLS:
      if l.isColumnbreak or l.LS_CL_missing:
         if l.isIndented or not LN_CL_eq_MOLEN:
            return 'C','paragraph' #C
         else:
            return 'D','linefeed' #D

      if l.NL_AVLS_eq_1:
         return 'E','paragraph' #E
      if LS_CL_leq_AVLS:
         return 'F','linefeed' #F
      if l.LS_CL_gt_AVLS:
         return 'G','paragraph' #G
      return 'Q','BUG!!!' #Q

   if not LN_PL_eq_MOLEN and l.LS_CL_eq_MOLS:
      if l.isIndented and l.LS_NL_eq_MOLS:
         return 'H','paragraph' #H
      else:
         return 'I','explicitlinefeed' #I

   misc.foo_assert( "not LN_PL_eq_MOLEN and not l.LS_CL_eq_MOLS", globals(), locals() )

   if l.isColumnbreak or l.LS_CL_missing:
      return 'J','paragraph' #J
   if l.NL_AVLS_eq_1:
      return 'K','explicitlinefeed' #K
   if l.LS_CL_lt_AVLS:
      return 'L','explicitlinefeed' #L
   if l.LS_CL_exactly_AVLS:
      if l.LS_CL_eq_LS_NL:
         return 'M','explicitlinefeed' #M
      else:
         return 'M','paragraph' #N
   if l.LS_CL_gt_AVLS:
      return 'O','paragraph' #O
   return 'P','BUG!!' #P

def predictDocument(document):
   lpll = None
   for page in document:
      i = 0
      while i < len(page):
         line = page[i]
         (line.algoCase,line.prediction) = predict(line)
         if not page.dehyphenateLine(i, lpll):
            # if a line was deleted, we are already at the next line
            # so we don't need to next i.
            i=i+1
      lpll = page[-1]
        
                                

                          
