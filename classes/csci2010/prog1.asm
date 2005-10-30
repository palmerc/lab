TITLE Program prog1               (prog1.asm)

; Program Description: Program 1
; Author: Cameron L Palmer
; Creation Date: 9/22/04
; Revisions:
; Date:  Modified by:

INCLUDE Irvine32.inc
.data
   ; (insert executable code here)
   prompt BYTE "Enter an integer: ",0
.code
main PROC
   ; (insert executable instructions here)
   call Clrscr
LOOPER:
   mov edx,OFFSET prompt
   call WriteString
   call ReadInt
   call WriteHex
   call Crlf
   jmp LOOPER
main ENDP

   ; (insert additional procedures here)
END main