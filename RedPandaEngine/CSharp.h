#pragma once
#include "Scripting_Language.h"
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

class CSharp :public Scripting_Languge {
    MonoDomain* domain;
    MonoAssembly* assembly;
    bool Init() {
        domain = mono_jit_init_version("myapp", "v4.0.30319");
    }

    bool Deconstruct() {
        
    }
};