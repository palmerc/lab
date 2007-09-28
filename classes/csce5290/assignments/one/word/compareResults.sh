#!/bin/bash

COUNT=`diff ../originals/LangId.sol.utf8 ./wordLangId.out | grep '<' | wc -l` 
PERCENT=$(( (300 - COUNT) * 100 / 300 ))
echo "Correct language identified ${PERCENT}% of the time"
 
