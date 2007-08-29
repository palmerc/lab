TITLE Keyboard Display

INCLUDE Irvine16.inc
.code

main PROC
  mov ax,@data
  mov ds,ax
  call ClrScr

  mov ax,40h
  mov es,ax
  mov ax,es:[17h]
  or  al,01111111b
  mov es:[17h],ax
	
L1:mov ah,10h
  int 16h
  call DumpRegs
  cmp al,1Bh
  jne L1

  call ClrScr
  exit

main ENDP
END main