#!/bin/bash
UNTBASE=`pwd`
UNTBIN=$UNTBASE/bin
UNTDATA=$UNTBASE/data
cd $UNTBIN
python $UNTBIN/untBuildingDownload.py > $UNTDATA/buildings.csv
python $UNTBIN/untClassScheduleDownload.py $UNTBASE
python $UNTBIN/untPDFTranslate.py $UNTBASE
