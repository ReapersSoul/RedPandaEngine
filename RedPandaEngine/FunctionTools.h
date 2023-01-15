#pragma once
#include <windows.h>
extern "C" void* __fastcall FT_CallFunction(int NumArgs, void* Function);
extern "C" void* __fastcall FT_PushIntPointer(int ArgOrPointer);
//extern "C" void* __fastcall FT_PushFloatDouble(double ArgOrPointer);
