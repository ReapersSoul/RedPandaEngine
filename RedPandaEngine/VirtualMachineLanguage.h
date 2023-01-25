#pragma once
#include "Language.h"
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <tuple>
#include <iostream>
#include <fstream>
#include <sstream>


class VirtualMachineLanguage : public Language {
public:
	//constructor
	VirtualMachineLanguage() {
		//constructor
	}
	//deconstructor
	~VirtualMachineLanguage() {
		//deconstructor
	}
	//methods
	virtual bool SetInt(std::string Name, int Value) = 0;
	virtual bool SetFloat(std::string Name, float Value) = 0;
	virtual bool SetDouble(std::string Name, double Value) = 0;
	virtual bool SetBool(std::string Name, bool Value) = 0;
	virtual bool SetString(std::string Name, std::string Value) = 0;
	virtual bool GetInt(std::string Name, int* Value) = 0;
	virtual bool GetFloat(std::string Name, float* Value) = 0;
	virtual bool GetDouble(std::string Name, double* Value) = 0;
	virtual bool GetBool(std::string Name, bool* Value) = 0;
	virtual bool GetString(std::string Name, std::string* Value) = 0;
private:

};