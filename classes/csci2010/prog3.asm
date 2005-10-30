TITLE Program 3 Indirect element swapping    (prog3.asm)

INCLUDE Irvine32.inc

.data
   elArray BYTE 50 DUP(?)
   elStart DWORD ?
   elMint DWORD ?
   elSize DWORD ?
	
   dataPrompt BYTE "Enter a string that you want reversed: ",0
   startPrompt BYTE "This is the starting value: ",0
   endPrompt BYTE "This is the ending value: ",0
   sizePrompt BYTE "This is the TYPE size of an element: ",0
   mintPrompt BYTE "This is the number of elements: ",0
   loopPrompt BYTE "Loop number ",0
   swapPrompt BYTE "Swapping ",0
   andPrompt BYTE " and ",0

.code	
main PROC
	;; Read in a string
   mov edx,OFFSET dataPrompt
   call WriteString
   mov edx,OFFSET elArray
   mov ecx,(SIZEOF elArray) - 1
   call ReadString
   mov elMint,eax

	;; Display the string
   mov edx,OFFSET startPrompt
   call WriteString
   mov edx,OFFSET elArray
   call WriteString
   call Crlf
   call Crlf

	;; Display the size of an element
   mov edx,OFFSET sizePrompt
   call WriteString
   mov elSize,TYPE elArray	; Save the size of an element
   mov eax,elSize
   call WriteDec
   call Crlf

	;; Display the number of elements
   mov edx,OFFSET mintPrompt
   call WriteString
   mov eax,elMint
   call WriteDec
   call Crlf

	;; Pre-loop setop
   mov elStart,OFFSET elArray
   mov esi,elStart	; Place starting location of the array in ESI
   mov ecx,elMint	; Copy the total number elements into the ECX Counter
   shr ecx,1			; Divide ECX by 2
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
   xchg al,[edi] 		; Exchange value EDI with EAX
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