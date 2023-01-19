#pragma once
#include "Scripting_Language.h"

void CoutColor(std::string str, int r = 255, int g = 255, int b = 255) {
    std::cout << "\x1b[38;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m" + str + "\x1b[0m";
}
void SetColor(int r, int g, int b) {
    std::cout << "\x1b[38;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m";
}
void ResetColor() {
    std::cout << "\x1b[0m";
}

class Scripting_Language_Manager :private Scripting_Language {
	std::vector<Scripting_Language*> langs;
    int CurrentConsole = -1;
	bool LiveConsoleLoop = true;
	bool LiveConsoleInstance = true;
	bool PauseOutput = false;
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

	void LiveConsoleExit() {
		LiveConsoleLoop = false;
	}

	void RunLiveConsole() {
        //setup Live Terminal	
		SetVar<bool>("Exit", false);
        //run console
        while (LiveConsoleLoop) {                
                CoutColor("Select Live Console (", 255, 51, 51);
				for (int i = 0; i < langs.size(); i++)
				{
					if (i < langs.size() - 1)
						CoutColor(langs[i]->GetName() + ", ", langs[i]->GetColor().r, langs[i]->GetColor().g, langs[i]->GetColor().b);
					else
						CoutColor(langs[i]->GetName(), langs[i]->GetColor().r, langs[i]->GetColor().g, langs[i]->GetColor().b);
				}
                CoutColor("): ", 255, 51, 51);
				std::string s;
				std::getline(std::cin, s);
				for (int i = 0; i < langs.size(); i++)
				{
					if (s == langs[i]->GetName()) {
						CurrentConsole = i;
						break;
					}
				}
				if (CurrentConsole == -1) {
					CoutColor("Not An Avalible Language!\n", 255, 0, 0);
					continue;
				}
				
				LiveConsoleInstance = true;
				while (LiveConsoleInstance)
				{
					try {
						s;
						CoutColor(langs[CurrentConsole]->GetName()+": ", 242, 237, 86);
						std::getline(std::cin, s);
						SetColor(40, 177, 249);
						langs[CurrentConsole]->RunString(s);
						ResetColor();
						LiveConsoleInstance = langs[CurrentConsole]->GetVarAsBool("Exit");
						while (PauseOutput) {
							std::this_thread::sleep_for(std::chrono::milliseconds(100));
						}
					}
					catch (Scripting_Language::Exception e) {
						std::cout << e.Id << "\n" << e.Desc << "\n";
					}
				}
				CoutColor(langs[CurrentConsole]->GetName() + " Live Terminal Exited!!\n", 102, 255, 102);
        }
	}

	//set pause output
	void SetPauseOutput(bool b) {
		PauseOutput = b;
	}

	////TODO::
	bool RegisterFunction(std::string Name, void* f) {
		for (int i = 0; i < langs.size(); i++)
		{
			if (!langs[i]->RegisterFunction(Name, f)) {
				return false;
			}
		}
		return true;
	}

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