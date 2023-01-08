#pragma once
#include "Scripting_Language.h"

class Scripting_Language_Manager :private Scripting_Language {
	std::vector<Scripting_Language*> langs;

public:

	void RegisterLanguage(Scripting_Language* sl) {
		langs.push_back(sl);
	}

	bool Init() {
		for (int i = 0; i < langs.size(); i++)
		{
			if (!langs[i]->Init()) {
				return false;
			}
		}
		return true;
	}

	bool Deconstruct() {
		for (int i = 0; i < langs.size(); i++)
		{
			if (!langs[i]->Deconstruct()) {
				return false;
			}
		}
		return true;
	}



	////TODO::
	//template<typename T>
	//bool RegisterFunction(std::string Name, std::function<T(std::vector<Var>*)>* f) {
	//	for (int i = 0; i < langs.size(); i++)
	//	{
	//		if (!langs[i]->RegisterFunction<T>(Name, f)) {
	//			return false;
	//		}
	//	}
	//	return true;
	//}

	template<typename T>
	bool SetVar(std::string Name, T value) {
		for (int i = 0; i < langs.size(); i++)
		{
			if (!langs[i]->SetVar(Name, value)) {
				return false;
			}
		}
		return true;
	}

	////TODO::
	//template<typename T>
	//bool RegisterLinkedVar(std::string Name, T* value) {
	//	for (int i = 0; i < langs.size(); i++)
	//	{
	//		if (!langs[i]->RegisterLinkedVar<T>(Name, value)) {
	//			return false;
	//		}
	//	}
	//	return true;
	//}
};