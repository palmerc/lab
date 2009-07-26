#!/bin/bash

FILE=computers_in_dcs

latex $FILE
bibtex $FILE
latex $FILE
latex $FILE
dvips -t letter $FILE.dvi -o $FILE.ps
ps2pdf $FILE.ps
rm $FILE.log $FILE.aux $FILE.dvi $FILE.ps
