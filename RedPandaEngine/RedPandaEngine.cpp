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

Scripting_Language_Manager SLM;
Lua lua;
Python python;
std::map<std::string, Toy> Toys;

std::string SelectedDevice = "";

class lovenseHandler:public ILovenseSDKNotify {
    /*Call when toy search start*/
    void LovenseDidSearchStart() {
        CoutColor("Search Started!\n");
    };

    /*Call when toy searching toy*/
    void LovenseSearchingToys(lovense_toy_info_t* info) {
        Toy t;
        t.SetToyManager(toyManager);
        t.SetID(std::string(info->toy_id));
        t.SetBatteryLevel(info->toy_battery);
        t.SetDeviceType((LVSToyType)info->toy_type);
        t.SetName(info->toy_name);
        lua.SetVar(info->toy_id, t.ToTable());
        Toys.insert(std::pair<std::string, Toy>(info->toy_id, t));
        CoutColor("\n");
        CoutColor("Toy ID: ");
        CoutColor(info->toy_id,0,255,0);
        SelectedDevice = info->toy_id;
        CoutColor("\n");
		toyManager->StopSearchToy();
    };

    /*Call when something went wrong*/
    void LovenseErrorOutPut(int errorCode, const char* errorMsg) {
        //CoutColor("Error Code: "+std::to_string(errorCode)+" "+errorMsg+"\n");
    };


    /*Call when toy search end*/
    void LovenseDidSearchEnd() {
        CoutColor("Search Ended!\n");
		SLM.SetPauseOutput(false);
    };

    /*Call when send cmd start*/
    void LovenseDidSendCmdStart() {
        //CoutColor("Sent Command Started!\n");
    };

    /*Call when send cmd return*/
    void LovenseSendCmdResult(const char* szToyID, CLovenseToy::CmdType cmd, const char* result, CLovenseToy::Error errorCode) {
        CoutColor("Toy: " + std::string(szToyID) + " Command: " + std::to_string(cmd) + " result: " + result + " Error Code: " + std::to_string(errorCode) + "\n");
        switch (cmd)
        {
        case CLovenseToy::CmdType::COMMAND_VIBRATE:
			//Toys.find(szToyID)->second.SetVibrationLevel()
			break;
        default:
            break;
        }
    };

    /*Call when send cmd end*/
    void LovenseDidSendCmdEnd() {
        //CoutColor("Sent Command End!\n");
    };

    /*Call when toy connected, or disconnected*/
    void LovenseToyConnectedStatus(const char* szToyID, bool isConnected) {
        CoutColor("Toy: " + std::string(szToyID) + " Connected: " + (isConnected? "True":"False"));
        Toys.find(szToyID)->second.SetConnected(isConnected);
    };
};

void printx(int a,int b,int c,int d,int e,int f,int g,int h) {
    printf("%d %d %d %d %d %d %d %d\n", a,b,c,d,e,f,g,h);
}
void printstr(char* str) {
    printf(str);
}

void Search() {
    toyManager->StartSearchToy();
    SLM.SetPauseOutput(true);
}

void Connect() {
    auto toy = Toys.find(SelectedDevice);
    if ( toy != Toys.end()) {
        toy->second.RequestConnect();
    }
}

void Vibrate(int level,int seconds) {
    auto toy = Toys.find(SelectedDevice);
    if (toy != Toys.end()) {
        //request vibration
        toy->second.RequestVibrate(level);
        //sleep for seconds
		std::this_thread::sleep_for(std::chrono::seconds(seconds));
        //request stop vibration
		toy->second.RequestVibrate(0);
    }
}


int main()
{
	
    FT_StartCall();
    FT_PushIntPointer((void*)"Hello World!\n");
    FT_CallFunction((void*)printstr);
    FT_StartCall();
    FT_PushIntPointer((void*)8);
    FT_PushIntPointer((void*)7);
    FT_PushIntPointer((void*)6);
    FT_PushIntPointer((void*)5);
    FT_PushIntPointer((void*)4);
    FT_PushIntPointer((void*)3);
    FT_PushIntPointer((void*)2);
    FT_PushIntPointer((void*)1);
    FT_CallFunction((void*)printx);

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
        lua.RegisterFunction("printx", (void*)printx);
        lua.RegisterFunction("printstr", (void*)printstr);
        lua.RegisterFunction("Search", (void*)Search);
        lua.RegisterFunction("Connect", (void*)Connect);
		lua.RegisterFunction("Vibrate", (void*)Vibrate);


        ////run code in languages
        //lua.RunString("print(SUB(1000,200))");
        //lua.RunString("print(FalStr)");
        //lua.RunString("print(FalNum-20)");

        SLM.RunLiveConsole();
    }
    catch (Scripting_Language::Exception e) {
        std::cout << e.Id << "\n" << e.Desc << "\n";
    }
    windowThread.join();
}