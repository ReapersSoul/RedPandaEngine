.data
NumArgs QWORD 0
ArgPointer QWORD 0
FunctPointer QWORD 0
.code
ALIGN 16

PushArg PROC
	push rcx
	ret
PushArg ENDP

CallFunction PROC
	pop r15
	mov NumArgs,rcx
	mov ArgPointer,rdx
	mov rdx,QWORD PTR[ArgPointer]
	mov rdx,QWORD PTR[rdx]
	mov ArgPointer,rdx
	mov FunctPointer,r8
	mov rcx,QWORD PTR[ArgPointer]
	mov rdx, QWORD PTR[ArgPointer-16]
	call FunctPointer
	push r15
	ret
CallFunction ENDP

END