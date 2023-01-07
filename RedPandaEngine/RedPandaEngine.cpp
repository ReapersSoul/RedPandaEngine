// RedPandaEngine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Graphics.h"
#include "Lua.h"

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

int main()
{
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
    Lua lua;
    lua.Init();
    try {
        lua.RegisterVar("Fal", "fuck");
        lua.RunString("print(Fal)");
        lua.SetVar("Fal", 100);
        lua.RunString("print(Fal-20)");

        int i = lua.GetVar<int>("Fal");
        std::cout << i << "\n";

        lua.RegisterFunction<double>("ADD", new std::function<double(std::vector<Scripting_Language::Var>*)>([](std::vector<Scripting_Language::Var>* vars) {
            double one = std::get<1>((*vars)[0]);
            double two = std::get<1>((*vars)[1]);
            return one+ two;
            }));
        lua.RegisterFunction<double>("SUB", new std::function<double(std::vector<Scripting_Language::Var>*)>([](std::vector<Scripting_Language::Var>* vars) {
            double one = std::get<1>((*vars)[0]);
            double two = std::get<1>((*vars)[1]);
            return one - two;
            }));

        lua.RunString("print(SUB(1000,200))");

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
                        std::getline(std::cin, s);
                        lua.RunString(s);
                    }
                    catch (Scripting_Language::Exception e) {
                        std::cout << e.Id << "\n" << e.Desc << "\n";
                    }
                }
                std::cout << "Lua Live Terminal Exited!!" << std::endl;
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
    windowThread.join();
}