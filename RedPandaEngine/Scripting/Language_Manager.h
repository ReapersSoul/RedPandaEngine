#pragma once
#include "util.h"
#include "Language.h"
#include <time.h>

void CoutColor(std::string str, int r = 255, int g = 255, int b = 255) {
    std::cout << "\x1b[38;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m" + str + "\x1b[0m";
}
void SetColor(int r, int g, int b) {
    std::cout << "\x1b[38;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m";
}
void ResetColor() {
    std::cout << "\x1b[0m";
}

class Language_Manager :private Language {
	std::vector<Language*> langs;
    int CurrentConsole = -1;
	bool LiveConsoleLoop = true;
	bool LiveConsoleInstance = true;
	bool PauseOutput = false;
public:

	void RegisterLanguage(Language* sl) {
		langs.push_back(sl);
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
					glm::vec3 color = langs[i]->GetPrimaryColor();
					if (i < langs.size() - 1)
						CoutColor(langs[i]->GetName() + ", ",color.r, color.g, color.b);
					else
						CoutColor(langs[i]->GetName(), color.r, color.g, color.b);
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
						glm::vec3 color = langs[CurrentConsole]->GetPrimaryColor();
						CoutColor(langs[CurrentConsole]->GetName() + ": ", color.r, color.g, color.b);
						std::getline(std::cin, s);
						SetColor(40, 177, 249);
						//langs[CurrentConsole]->RunString(s);
						ResetColor();
						//LiveConsoleInstance = !langs[CurrentConsole]->GetVarAsBool("Exit");
						/*while (PauseOutput) {
							std::this_thread::sleep_for(std::chrono::milliseconds(100));
						}*/
					}
					catch (Exception e) {
						e.Print();
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
			//if (!langs[i]->SetVar(Name, value)) {
			//	return false;
			//}
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