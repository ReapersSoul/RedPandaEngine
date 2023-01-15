.data
;DWORD val
.code
ALIGN 16

PushArg PROC
	push rcx
	ret
PushArg ENDP

CallFunction PROC
	pop rcx
	call rcx
	push rsp
	ret
CallFunction ENDP

END