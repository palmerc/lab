TITLE Program Number One  (progbase.asm)

INCLUDE Irvine32.inc
.code
main PROC
   
   mov eax,10000h             ; EAX = 10000h
   add eax,40000h             ; EAX = 50000h
   sub eax,20000h             ; EAX = 30000h
   call DumpRegs              ; display registers

   exit
main ENDP
END main

