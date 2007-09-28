#!/bin/bash

for lang in French Italian English
do
	./wordModelLangId.pl ../originals/LangId.train.$lang.utf8 > $lang.model
done 
