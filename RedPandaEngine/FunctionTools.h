#pragma once
#include <windows.h>
extern "C" void __fastcall PushArg(void* arg);

extern "C" void* __fastcall CallFunction(void* function);
