#pragma once
#include <windows.h>
extern "C" void* __fastcall FT_CallFunction(void* Function);
extern "C" void __fastcall FT_PushIntPointer(void* ArgOrPointer);
extern "C" void __fastcall FT_StartCall();
extern "C" void __fastcall FT_EndCall(void* Arg);
/*

std::vector<void*> Stack;

extern "C" void __fastcall FT_PushIntPointer(void* ArgOrPointer) {
	Stack.push_back(ArgOrPointer);
}

extern "C" void __fastcall FT_StartCall() {
	Stack.clear();
}

extern "C" bool __fastcall FT_CanPop() {
	return Stack.size() > 0;
}

extern "C" void* __fastcall FT_PopIntPointer() {
	//check stack size
	if (Stack.size() == 0) {
		return nullptr;
	}
	void* i = (void*)Stack.back();
	Stack.pop_back();
	return i;
}
*/
