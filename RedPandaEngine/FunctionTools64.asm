.data
FunctionPointer QWORD 0
ReturnPointer QWORD 0
StackPointer QWORD 0

.code
ALIGN 16

FT_StartCall PROC
	mov [StackPointer],rsp
	ret
FT_StartCall ENDP
	
FT_PushIntPointer PROC
	pop ReturnPointer
	push rcx
	push ReturnPointer
	ret
FT_PushIntPointer ENDP
    
FT_CallFunction PROC
	;save return address in Memory
	pop ReturnPointer
	mov FunctionPointer,rcx
	pop rcx
	pop rdx
	pop r8
	pop r9
	push r9
	push r8
	push rdx
	push rcx
	call FunctionPointer
	mov rcx,0
	mov rdx,0
	mov r8,0
	mov r9,0
	mov rsp, [StackPointer]
	push ReturnPointer
	ret
FT_CallFunction ENDP

END