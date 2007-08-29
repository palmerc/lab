TITLE Lab 3 Indirect element swapping    (lab3.asm)

INCLUDE Irvine32.inc

.data
   array1 BYTE "abcdefghijklmnopqrstuvwxyz"
   elHalf = ($ - array1) / 2 
   elStart DWORD ?
   elMint DWORD ?
   elSize DWORD ?

   startPrompt BYTE "This is the starting value: ",0
   endPrompt BYTE "This is the ending value: ",0
   sizePrompt BYTE "This is the TYPE size of an element: ",0
   mintPrompt BYTE "This is the number of elements: ",0
   loopPrompt BYTE "Loop number ",0
   swapPrompt BYTE "Swapping ",0
   andPrompt BYTE " and ",0

.code	
main PROC
   mov edx,OFFSET startPrompt
   call WriteString
   mov edx,OFFSET array1
   call WriteString
   call Crlf
   call Crlf
		
   mov edx,OFFSET sizePrompt
   call WriteString
   mov elSize,TYPE array1	; Save the size of an element
   mov eax,elSize
   call WriteDec
   call Crlf

   mov edx,OFFSET mintPrompt
   call WriteString
   mov elMint,LENGTHOF array1	; Save the number of elements
   mov eax,elMint
   call WriteDec
   call Crlf

   mov elStart,OFFSET array1
   mov esi,elStart	; Place starting location of the array in ESI
   mov ecx,elHalf	; Copy half the number of elements into the ECX Counter
   mov edi,elMint		; Initialize EDI with number of elements
   add edi,esi			; Add starting location to number of elements
   sub edi,1			; Subtract one from number of elements

LOOPER:
	;; Print loop count
   mov edx,OFFSET loopPrompt
   call WriteString
   mov eax,ecx
   call WriteDec
   call Crlf
	;; Print swapped values
   mov edx,OFFSET swapPrompt
   call WriteString
   mov al,[esi]
   call WriteChar
   mov edx,OFFSET andPrompt
   call WriteString
   mov al,[edi]
   call WriteChar
   call Crlf
	
	;; Actual swapping of elements
   mov  al,[esi]		; Copy the value of ESI into eax
   xchg al,[edi]		; Exchange value EDI with EAX
   mov  [esi],al		; Place EAX back into ESI

	;; Increment and Decrement the two ends
   add esi,elSize
   sub edi,elSize
	;; Loop until ECX has been sucked dry
   loop LOOPER

	;; Print the reversed string
   call Crlf
   mov edx,OFFSET endPrompt
   call WriteString
   mov edx,elStart
   call WriteString
   call Crlf
	
   exit
	
main ENDP
END main