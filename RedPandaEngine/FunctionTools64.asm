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
	mov FunctPointer,r8
	mov rcx,QWORD PTR[ArgPointer]
	mov rdx, QWORD PTR[rcx-8]
	mov rcx, QWORD PTR[rcx]
	call FunctPointer
	push r15
	ret
CallFunction ENDP

END