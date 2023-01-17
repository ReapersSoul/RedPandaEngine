.data
FunctionPointer QWORD 0
PushedArgs QWORD 0
ArgCounter QWORD 0
ReturnPointer QWORD 0
ArgStack QWORD 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
ArgStackBeginPtr QWORD ArgStack
ArgStackPtr QWORD ArgStack
.code
ALIGN 16

FT_PushIntPointer PROC
	pop ReturnPointer
	mov QWORD PTR[ArgStackPtr],rcx
	add ArgStackPtr,1
	push ReturnPointer
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
			mov rcx,QWORD PTR [ArgStackPtr]
			call CheckLastArg
			mov rdx,QWORD PTR [ArgStackPtr]
			call CheckLastArg
			mov r8,QWORD PTR [ArgStackPtr]
			call CheckLastArg
			mov r9,QWORD PTR [ArgStackPtr]
			call CheckLastArg

		NextArg:
			push QWORD PTR [ArgStackPtr]
			call CheckLastArg
			jmp NextArg
				;call funciton

	CheckLastArg:
			sub ArgStackPtr,1
			push rcx
			mov rcx,ArgStackBeginPtr
			cmp ArgStackPtr,rcx
			pop rcx
			jle FunctionCall
			ret

	FunctionCall:
		mov PushedArgs,0
		call FunctionPointer
	push ReturnPointer
	ret
FT_CallFunction ENDP

END