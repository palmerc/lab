#!/bin/bash

FILE=hw4

latex $FILE
bibtex $FILE
latex $FILE
latex $FILE
dvips -t letter $FILE.dvi -o $FILE.ps
ps2pdf $FILE.ps
rm $FILE.log $FILE.aux $FILE.dvi $FILE.ps $FILE.blg $FILE.bbl

