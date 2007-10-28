#!/bin/bash

FILE=sample

latex $FILE.tex
dvips -t letter $FILE.dvi -o $FILE.ps
ps2pdf $FILE.ps
rm $FILE.log $FILE.aux $FILE.dvi $FILE.ps
