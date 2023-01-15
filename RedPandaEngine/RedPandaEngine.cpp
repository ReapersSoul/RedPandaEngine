// RedPandaEngine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include "Graphics.h"
#include "Lua.h"
#include "Python.h"
#include "Scripting_Language_Manager.h"
#include "Lovense_Device.h"

class PrimaryEventHandler :public EventStream::EventProcessor {
public:
    bool HandleEvent(EventStream::Event e) {
        if (e.mouse.button == GLFW_MOUSE_BUTTON_MIDDLE && e.mouse.action == GLFW_PRESS) {
            printf("middle mouse click");
            return false;
        }
        /*if (e.mouse.button == GLFW_MOUSE_BUTTON_1 && e.mouse.action == GLFW_PRESS) {
            printf("Left mouse click");
            return false;
        }*/
        return true;
    };
};

//globals
Graphics::Window wind("RedPanda v0.0.1");
PrimaryEventHandler PEH;
//draw functions
void GUI(GLFWwindow* wind, int Window_Width, int Window_Height) {
    //GUI HERE
    
}

void Camera(GLFWwindow* wind, int Window_Width, int Window_Height) {
    //CAMERA STUFF
}

void Draw(GLFWwindow* wind, int Window_Width, int Window_Height) {
    //DRAW CALL

}

enum ScriptingConsoleType {
    SCT_LUA,
    SCT_JS,
    SCT_JAVA,
    SCT_CSHARP,
    SCT_PYTHON,
    SCT_NATIVE,
    SCT_NONE
};

void CoutColor(std::string str, int r=255, int g=255, int b=255) {
    std::cout<<"\x1b[38;2;"+std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m" + str + "\x1b[0m";
}
void SetColor(int r, int g, int b) {
    std::cout << "\x1b[38;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m";
}

void ResetColor() {
    std::cout << "\x1b[0m";
}

Scripting_Language_Manager SLM;
Lua lua;
Python python;
std::map<std::string, Toy> Toys;


class lovenseHandler:public ILovenseSDKNotify {
    /*Call when toy search start*/
    void LovenseDidSearchStart() {
        CoutColor("Search Started!");
    };

    /*Call when toy searching toy*/
    void LovenseSearchingToys(lovense_toy_info_t* info) {
        Toy t;
        t.SetToyManager(toyManager);
        t.SetID(std::string(info->toy_id));
        t.SetBatteryLevel(info->toy_battery);
        t.SetDeviceType((LVSToyType)info->toy_type);
        t.SetDeviceName(std::string(info->toy_name));
        Toys.insert(std::pair<std::string, Toy>(info->toy_id, t));
        CoutColor(info->toy_id);
    };

    /*Call when something went wrong*/
    void LovenseErrorOutPut(int errorCode, const char* errorMsg) {
        CoutColor("Error Code: "+std::to_string(errorCode)+" "+errorMsg);
    };


    /*Call when toy search end*/
    void LovenseDidSearchEnd() {
        CoutColor("Search Ended!");
    };

    /*Call when send cmd start*/
    void LovenseDidSendCmdStart() {
        CoutColor("Sent Command Started!");
    };

    /*Call when send cmd return*/
    void LovenseSendCmdResult(const char* szToyID, CLovenseToy::CmdType cmd, const char* result, CLovenseToy::Error errorCode) {
        CoutColor("Toy: "+std::string(szToyID)+" Command: "+std::to_string(cmd)+" result: "+result+" Error Code: "+std::to_string(errorCode));
    };

    /*Call when send cmd end*/
    void LovenseDidSendCmdEnd() {
        CoutColor("Sent Command End!");
    };

    /*Call when toy connected, or disconnected*/
    void LovenseToyConnectedStatus(const char* szToyID, bool isConnected) {
        CoutColor("Toy: " + std::string(szToyID) + " is Connected: " + std::to_string(isConnected));
    };
};

void printx(int x,int l) {
    printf("%d %d\n", x,l);
}

int main()
{
    int i[] = { 8, 10 };

    CallFunction(1, (void*)i, (void*)printx);

    toyManager = GetLovenseToyManager();
    toyManager->SetDeveloperToken("SCfh7CamO5irTBgvB53z2hdAYjmq2SJLiUNFLrB1SvOQu9hyemQL5lUimQZTeIqT");
    toyManager->RegisterEventCallBack(new lovenseHandler());
    std::thread windowThread = std::thread([]() {
        SetCallBackWindow(&wind);
        wind.Set_Camera_function(Camera);
        wind.Set_Draw_function(Draw);
        wind.Set_GUI_function(GUI);
        wind.Init();
        wind.AddEventProcessor(&PEH);

        wind.Loop();

        wind.CleanUp();
        });

    SLM.RegisterLanguage(&lua);
    SLM.RegisterLanguage(&python);

    SLM.Init();
    try {
        //setup Language Manager and register languages


        //register variables in languages
        //SLM.SetVar("FalStr", "fuck");
        //SLM.SetVar("FalNum", 100);
        SLM.SetVar("___Live_Terminal_Run___", true);

        //register functions in languages
        //SLM.RegisterFunction<double>("ADD", new std::function<double(std::vector<Scripting_Language::Var>*)>([](std::vector<Scripting_Language::Var>* vars) {
        //    double one = std::get<1>((*vars)[0]);
        //    double two = std::get<1>((*vars)[1]);
        //    return one+ two;
        //    }));
        //SLM.RegisterFunction<double>("SUB", new std::function<double(std::vector<Scripting_Language::Var>*)>([](std::vector<Scripting_Language::Var>* vars) {
        //    double one = std::get<1>((*vars)[0]);
        //    double two = std::get<1>((*vars)[1]);
        //    return one - two;
        //    }));
        lua.RegisterFunction<int,bool(*)(std::string)>("Connect", 1, [](std::string ID){
            Toys.find(ID);
            return false;
            });


        ////run code in languages
        //lua.RunString("print(SUB(1000,200))");
        //lua.RunString("print(FalStr)");
        //lua.RunString("print(FalNum-20)");

        //setup Live Terminal
        lua.RunString("_G.Exit=function() _G.___Live_Terminal_Run___=false end");
        ScriptingConsoleType sct=SCT_NONE;
        bool LiveConsole=true;
        while (LiveConsole) {
            std::string tmp;
            switch (sct)
            {
            case SCT_LUA:
                while (lua.GetVarAsBool("___Live_Terminal_Run___"))
                {
                    try {
                        std::string s;
                        CoutColor("LUA: ",242, 237, 86);
                        std::getline(std::cin, s);
                        SetColor(40, 177, 249);
                        lua.RunString(s);
                        ResetColor();
                    }
                    catch (Scripting_Language::Exception e) {
                        std::cout << e.Id << "\n" << e.Desc << "\n";
                    }
                }
                CoutColor("Lua Live Terminal Exited!!\n", 102, 255, 102);
                sct = SCT_NONE;
                break;
            case SCT_JS:
                break;
            case SCT_JAVA:
                break;
            case SCT_CSHARP:
                break;
            case SCT_PYTHON:
                while (python.GetVarAsBool("___Live_Terminal_Run___"))
                {
                    try {
                        std::string s;
                        CoutColor("LUA: ", 0, 102, 204);
                        std::getline(std::cin, s);
                        SetColor(255, 255, 102);
                        lua.RunString(s);
                        ResetColor();
                    }
                    catch (Scripting_Language::Exception e) {
                        std::cout << e.Id << "\n" << e.Desc << "\n";
                    }
                }
                CoutColor("Lua Live Terminal Exited!!\n", 102, 255, 102);
                sct = SCT_NONE;
                break;
            case SCT_NATIVE:
                break;
            case SCT_NONE:
                CoutColor("Select Live Console (", 255, 51, 51); 
                CoutColor("LUA,", 242, 237, 86); 
                CoutColor(" JS,"); 
                CoutColor(" JAVA,"); 
                CoutColor(" CSHARP,"); 
                CoutColor(" PYTHON,",0,102,204); 
                CoutColor(" NATIVE");
                CoutColor("): ", 255, 51, 51);
                std::getline(std::cin, tmp);
                if (tmp == "Exit()") {
                    LiveConsole=false;
                }else if (tmp == "LUA") {
                    sct = SCT_LUA;
                }
                else if (tmp == "PYTHON") {
                    sct = SCT_PYTHON;
                }
                break;
            default:
                break;
            }
        }
    }
    catch (Scripting_Language::Exception e) {
        std::cout << e.Id << "\n" << e.Desc << "\n";
    }
    windowThread.join();
}