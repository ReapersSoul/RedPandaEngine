// RedPandaEngine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Graphics.h"
#include "Lua.h"
#include "Scripting_Language_Manager.h"

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

int main()
{
    //std::thread windowThread = std::thread([]() {
    //    SetCallBackWindow(&wind);
    //    wind.Set_Camera_function(Camera);
    //    wind.Set_Draw_function(Draw);
    //    wind.Set_GUI_function(GUI);
    //    wind.Init();
    //    wind.AddEventProcessor(&PEH);

    //    wind.Loop();

    //    wind.CleanUp();
    //    });
    Scripting_Language_Manager SLM;
    Lua lua;
    SLM.RegisterLanguage(&lua);
    lua.Init();
    try {
        //setup Language Manager and register languages


        //register variables in languages
        SLM.RegisterVar("FalStr", "fuck");
        SLM.SetVar("FalNum", 100);

        //register functions in languages
        SLM.RegisterFunction<double>("ADD", new std::function<double(std::vector<Scripting_Language::Var>*)>([](std::vector<Scripting_Language::Var>* vars) {
            double one = std::get<1>((*vars)[0]);
            double two = std::get<1>((*vars)[1]);
            return one+ two;
            }));
        SLM.RegisterFunction<double>("SUB", new std::function<double(std::vector<Scripting_Language::Var>*)>([](std::vector<Scripting_Language::Var>* vars) {
            double one = std::get<1>((*vars)[0]);
            double two = std::get<1>((*vars)[1]);
            return one - two;
            }));


        ////run code in languages
        //lua.RunString("print(SUB(1000,200))");
        //lua.RunString("print(FalStr)");
        //lua.RunString("print(FalNum-20)");

        //setup Live Terminal
        lua.RunString("_G.Exit=function() _G.___Live_Terminal_Run___=false end");
        lua.SetVar<bool>("___Live_Terminal_Run___", true);
        ScriptingConsoleType sct=SCT_NONE;
        bool LiveConsole=true;
        while (LiveConsole) {
            std::string tmp;
            switch (sct)
            {
            case SCT_LUA:
                while (lua.GetVar<bool>("___Live_Terminal_Run___"))
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
                break;
            case SCT_NATIVE:
                break;
            case SCT_NONE:
                CoutColor("Select Live Console (", 255, 51, 51); 
                CoutColor("LUA,", 242, 237, 86); 
                CoutColor(" JS,"); 
                CoutColor(" JAVA,"); 
                CoutColor(" CSHARP,"); 
                CoutColor(" PYTHON,"); 
                CoutColor(" NATIVE");
                CoutColor("): ", 255, 51, 51);
                std::getline(std::cin, tmp);
                if (tmp == "Exit()") {
                    LiveConsole=false;
                }else if (tmp == "LUA") {
                    sct = SCT_LUA;
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
    //windowThread.join();
}