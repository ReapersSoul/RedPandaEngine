.data
ArgOffset QWORD 20h
FunctionPointer QWORD 0
PushedArgs QWORD 0
ArgCounter QWORD 0
StackCounter QWORD 0
TmpArg QWORD 0
StackPointer QWORD 0
ReturnPointer QWORD 0
.code
ALIGN 16

FT_PushIntPointer PROC
	pop rdx
	push rcx
	add PushedArgs,1
	push rdx
	ret
FT_PushIntPointer ENDP

FT_FinishPush PROC
	
FT_FinishPush ENDP

FT_CallFunction PROC
	;save return addressin in register 15
	pop ReturnPointer
	;save function pointer
	mov FunctionPointer, rcx
	
	;load args
	mov ArgCounter,0
	mov StackCounter,32
	loadArg:
		;jump table
		cmp ArgCounter,0
		je first
		cmp ArgCounter,1
		je second
		cmp ArgCounter,2
		je third
		cmp ArgCounter,3
		je fourth
		jmp FunctionCall

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
		NextArg:
			add ArgCounter,1
			jmp loadArg
				;call funciton
	FunctionCall:
		push r9
		push r8
		push rdx
		push rcx
		mov PushedArgs,0
		call FunctionPointer
	push ReturnPointer
	ret
FT_CallFunction ENDP

END