iPhoneInterface v0.3.1
========================
A tool to manipulate the iPhones state, launch services and interact with the chroot'd filesystem.

Requires iTunesMobileDevice.dll from C:\Program Files\Common Files\Apple\Mobile Device Support\bin

External Build Requirements
---------------------------
QuickTime SDK (https://connect.apple.com/cgi-bin/WebObjects/MemberSite.woa/wa/getSoftware?bundleID=19636)
iTunes v7.3
mingw + msys (and optionally gdb)
	http://downloads.sourceforge.net/mingw/MinGW-5.1.3.exe?modtime=1168794334&big_mirror=1
	http://downloads.sourceforge.net/mingw/MSYS-1.0.10.exe?modtime=1079395200&big_mirror=1
	http://downloads.sourceforge.net/mingw/gdb-5.2.1-1.exe?modtime=1045353600&big_mirror=1

xCode (Mac only)

Building on Windows
-------------------
Then use the dev-c++ project or run make -f Makefile.win

Building on OSX
---------------
For PPC-only build issue:
make -f Makefile.osx

For a universal build, issue:
make -f makefile.osx UNIVERSAL=1

 - Warren 7/6/2007
 - ixtli & nall 7/7/7