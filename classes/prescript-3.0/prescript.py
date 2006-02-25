#! /usr/bin/python
# -*-Python-*-
## 
##     prescript 2.2-- a PostScript to text converter
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

# Change history:
#        February 24, 2006: Started to make prescript compatible Python 2.4
#        April 30, 1999: Modified Prescript source to be compatible with Python 1.5
#        

import io, process, predict
import os, sys, misc, string, re
from misc import msg


def setPrescriptDir(): 
   global prescript_dir

   # find where prescript is, and check the format of the string
   if 'PRESCRIPT_DIR' in os.environ.keys():
      prescript_dir = os.environ['PRESCRIPT_DIR']
   else: 
      prescript_dir = ''
      
   if prescript_dir == '': 
      prescript_dir = os.path.split(sys.argv[0])[0]
      
   if prescript_dir == '':
      prescript_dir = './'
      
   if prescript_dir[-1] != '/':
      prescript_dir = prescript_dir + '/'


def makeFormatter(outFilename, format):
   if outFilename == '-':
      outFile = sys.stdout
   else:
      outFile = open(outFilename, "w")
      
   return io.newFormatter(format, outFile)


def checkParams(argv):
   # check we have all commandline parameters
   if len(argv) < 3:
      msg( "Usage: prescript <plain|html|arff> <input> [output]" )
      sys.exit(1)

   inputFilename, format = argv[2],argv[1]

   # append the .ps if it was omitted
   if re.search(r'\.ps$', inputFilename) == -1: 
      inputFilename=inputFilename+'.ps'
      
   # plain is a more convenient word to use
   if format == 'plain': 
      format = 'txt'
      
   # check that it's a valid format
   if not format in io.knownFormats:
      msg( "Unknown format %s." % format )
      sys.exit(1)

   # make sure it exists
   if not os.path.exists( inputFilename ):
      msg( "Can't find (or access) file '%s'" % inputFilename )
      sys.exit(1)

   if len(sys.argv) == 4:
      outFilename = sys.argv[3]
   else:
      outFilename = misc.MakeFilename(inputFilename, '.'+format)
   
   return format,inputFilename,outFilename


if __name__ == '__main__':
   setPrescriptDir()
   (format,inputFilename,outFilename) = checkParams(sys.argv)
   
   # Real work begins
   frags = io.readFragments(inputFilename)
   document = process.assembleDocument( frags )
   
   #   del frags # Uncomment this if you are low on memory
   process.preprocessDocument( document )
   predict.predictDocument( document )
   if format == 'arff': 
      io.applyHandcheck(inputFilename, document )
   io.renderDocument( makeFormatter(outFilename, format), document )
