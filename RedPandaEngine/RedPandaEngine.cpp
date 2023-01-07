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

int main()
{
    Lua lua;
    lua.Init();
    try {
        lua.RegisterVar("Fal", "fuck");
        lua.RunString("print(Fal)");
        lua.SetVar("Fal", 100);
        lua.RunString("print(Fal-20)");

        int i = lua.GetVar<int>("Fal");
        std::cout << i << "\n";

        lua.RegisterFunction<double,2>("ADD", [](std::vector<Scripting_Languge::Var> vars) {
            return std::get<2>(vars[0])+ std::get<2>(vars[1]);
            });

        lua.RunString("print(ADD(10,200))");
    }
    catch (Scripting_Languge::Exception e) {
        std::cout << e.Id << "\n" << e.Desc << "\n";
    }
    SetCallBackWindow(&wind);
    wind.Set_Camera_function(Camera);
    wind.Set_Draw_function(Draw);
    wind.Set_GUI_function(GUI);
    wind.Init();
    wind.AddEventProcessor(&PEH);

    wind.Loop();

    wind.CleanUp();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
