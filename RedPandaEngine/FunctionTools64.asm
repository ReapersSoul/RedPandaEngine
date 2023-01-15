.data
NumArgs QWORD 0
FunctionPointer QWORD 0
ArgSizesPointer QWORD 0
.code
ALIGN 16

FT_PushIntPointer PROC
	pop r15
	push rcx
	push r15
	ret
FT_PushIntPointer ENDP

FT_CallFunction PROC
	;save return addressin in register 15
	pop r15
	;load arguments from stack
	mov NumArgs,rcx
	mov FunctionPointer, rdx
	pop rcx
	pop rdx
	pop r8
	pop r9
	;call funciton
	call FunctionPointer
	push r15
	ret
FT_CallFunction ENDP

END