TITLE Chapter 4 Test (chap4.asm)

INCLUDE Irvine32.inc

.code

main PROC
   mov ax,7ff0h
   call DumpRegs
   add al,10h
   call DumpRegs
   add ah,1
   call DumpRegs
   add ax,2
   call DumpRegs
	
   exit
main ENDP
END main