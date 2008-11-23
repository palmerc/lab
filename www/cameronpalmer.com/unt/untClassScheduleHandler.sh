#!/bin/bash
UNTBASE=`pwd`
UNTBIN=$UNTBASE/bin
UNTDATA=$UNTBASE/data
cd $UNTBIN
python $UNTBIN/untClassScheduleDownload.py $UNTBASE
python $UNTBIN/untPDFTranslate.py $UNTBASE
