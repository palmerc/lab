#!/bin/bash

FILE=sample

latex $FILE.tex
# dvips will not respect letter size in the tex file it must be explicitly
# set here or you will get A4
dvips -t letter $FILE.dvi -o $FILE.ps
ps2pdf $FILE.ps
rm $FILE.log $FILE.aux $FILE.dvi $FILE.ps
