echo "setting up environment at base $PWD"

SCALE="$PWD"; export SCALE
SCALEHOME="$SCALE/scale"; export SCALEHOME
SCALEDOC="$SCALE/doc"; export SCALEDOC

SCALEHOST="$(uname)"; export SCALEHOST
SCALEHOSTNAME=""; export SCALEHOSTNAME
SCALEHOSTTYPE="i386"; export SCALEHOSTTYPE
SCALERELEASE=""; export SCALERELEASE
SCALETARGETTYPE="sparc"; export SCALETARGETTYPE

#SCALELIB="$SCALE/$SCALERELEASE/$SCALEHOSTNAME/lib"; export SCALELIB
#SCALEHOMELIB="$SCALE/$SCALERELEASE/$SCALEHOSTNAME/lib"; export SCALEHOMELIB
#SCALEBIN="$SCALE/$SCALERELEASE/$SCALEHOSTNAME/bin"; export SCALEBIN
#SCALEHOMEBIN="$SCALE/$SCALERELEASE/$SCALEHOSTNAME/bin"; export SCALEHOMEBIN
#LD_LIBRARY_PATH="$LD_LIBRARY_PATH:$SCALELIB:$SCALEHOMELIB"; export LD_LIBRARY_PATH

CLASSDEST="$SCALEHOME/classes"; export CLASSDEST
CLASSPATH="$CLASSPATH:$SCALE:$SCALEHOME/classes:$SCALEHOME/frontend/antlr.jar"; export CLASSPATH
JAVA="java"; export JAVA
JAVAC="javac"; export JAVAC
JAVACFLAGS="-J-Xmx256m -d $CLASSDEST -g"; export JAVACFLAGS

JAVAD="javadoc"; export JAVAD
JAVADDEST="$SCALEDIR/doc/html"; export JAVADDEST
JAVADFLAGS="-J-Xmx128m -J-Xms32m"; export JAVADFLAGS
JAVAH="javah"; export JAVAH

CC="cc"; export CC
CFLAGS="-O2"; export CFLAGS
CXX="c++"; export CXX
CXXFLAGS="-O2"; export CXXFLAGS
AR="ar"; export AR
