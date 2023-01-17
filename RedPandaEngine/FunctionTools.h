#pragma once
#include <windows.h>
extern "C" void* __fastcall FT_CallFunction(void* Function);
extern "C" void __fastcall FT_PushIntPointer(void* ArgOrPointer);
extern "C" void __fastcall FT_StartCall();
extern "C" void __fastcall FT_EndCall(void* Arg);

//extern "C" int __fastcall FT_GetAssembly(int v);
//extern "C" int __fastcall FT_SetAssembly(int v,int x);
//extern "C" void* __fastcall FT_PushFloatDouble(double ArgOrPointer);
