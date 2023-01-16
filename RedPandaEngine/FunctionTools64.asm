.data
ArgOffset QWORD 20h
FunctionPointer QWORD 0
PushedArgs QWORD 0
.code
ALIGN 16

FT_PushIntPointer PROC
	pop r15
	push rcx
	add PushedArgs,1
	push r15
	ret
FT_PushIntPointer ENDP

FT_CallFunction PROC
	;save return addressin in register 15
	pop r15
	;save function pointer
	mov FunctionPointer, rcx
	
	;load args
	mov r14,0
	mov r11,32
	loadArg:
		;jump table
		cmp r14,0
		je first
		cmp r14,1
		je second
		cmp r14,2
		je third
		cmp r14,3
		je fourth
		jmp AdditionalArgs

		first:
			pop rcx
			jmp NextArg
		second:
			pop rdx
			jmp NextArg
		third:
			pop r8
			jmp NextArg
		fourth:
			pop r9
			jmp NextArg
		AdditionalArgs:
			pop r13
			mov r12,rsp
			add r12,r11
			mov qword ptr [r12],r13
			add r11,8
		NextArg:
			add r14,1
			cmp r14,PushedArgs
			jne loadArg
				;call funciton
	FunctionCall:
		mov PushedArgs,0
		call FunctionPointer
	push r15
	ret
FT_CallFunction ENDP

END