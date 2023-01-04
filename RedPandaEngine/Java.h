#pragma once
#include "Scripting_Language.h"
#include <jni.h>

class Java :public Scripting_Languge {
	JavaVM* jvm;       /* denotes a Java VM */
	JNIEnv* env;       /* pointer to native method interface */
    JavaVMInitArgs vm_args;                        // Initialization arguments
    JavaVMOption* options;   // JVM invocation options
    jint rc;
public:
	bool Init() {
        options = new JavaVMOption[1];   // JVM invocation options
        options[0].optionString = (char*)"-Djava.class.path=.";   // where to find java .class
        vm_args.version = JNI_VERSION_1_6;             // minimum Java version
        vm_args.nOptions = 1;                          // number of options
        vm_args.options = options;
        vm_args.ignoreUnrecognized = false;     // invalid options make the JVM init fail
        //=============== load and initialize Java VM and JNI interface =============
        rc = JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);  // YES !!
        delete options;    // we then no longer need the initialisation options. 
        if (rc != JNI_OK) {
            // TO DO: error processing... 
            exit(EXIT_FAILURE);
        }
        //=============== Display JVM version =======================================
        //std::cout << "JVM load succeeded: Version ";
        jint ver = env->GetVersion();
        //cout << ((ver >> 16) & 0x0f) << "." << (ver & 0x0f) << endl;
	}

    bool Deconstruct() {
        jvm->DestroyJavaVM();
    }

    bool LoadScript(std::string path) {

    };
    bool LoadString(std::string str) {

    };
    bool RunScript(std::string path) {

    };
    bool RunString(std::string str) {
        //env->;
    };
    template<typename T>
    bool RegisterFunction(std::string Name, std::function<T> f) {
        //env->RegisterNatives
    }
    template<typename T>
    bool RegisterVar(std::string Name, T value);
    template<typename T>
    bool RegisterLinkedVar(std::string Name, T* value);
    template<typename T>
    bool SetVar(std::string Name, T val);
    template<typename T>
    T GetVar(std::string Name);
    template<typename T>
    T CallFunction(std::string Name, ...);
};