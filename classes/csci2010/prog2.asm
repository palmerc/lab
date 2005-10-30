TITLE Program 2 Fibonacci Sequence         (prog2.asm)

; Program Description: Program to calculate the fibonacci sequence
; Author: Cameron Palmer
; Creation Date: 9/26/04
; Revisions:
; Date:  Modified by:

INCLUDE Irvine32.inc
.data
   val1 DWORD 0
   val2 DWORD 1

.code
main PROC
   mov ecx,14
   mov eax,val2
   call WriteInt
   call Crlf
LOOPER:
   mov eax,val1
   add eax,val2
   call WriteInt
   call Crlf
   mov ebx,val2
   mov val1,ebx
   mov val2,eax   
   loop LOOPER
   exit
main ENDP
END main