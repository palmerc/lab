#!/bin/bash

for lang in French Italian English
do
	./gtModelLangId.pl ../originals/LangId.train.$lang.utf8 > $lang.model
done 
