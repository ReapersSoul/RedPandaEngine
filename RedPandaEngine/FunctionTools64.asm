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
	mov ArgPointer,r8
	mov FunctPointer,rdx

loopArgs:

	mov rcx, QWORD PTR[ArgPointer]
	add ArgPointer,1
	mov rdx, QWORD PTR[ArgPointer]

	mov rcx, QWORD PTR[rcx]
	mov rdx, QWORD PTR[rdx]

	call FunctPointer
	push r15
	ret
CallFunction ENDP

END