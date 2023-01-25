// RedPandaEngine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
//#include "Graphics.h"
//#include "Lua.h"
//#include "Python.h"
//#include "Language_Manager.h"
#include "Lovense_Device.h"

ToyManager tm;

////draw functions
//void GUI(GLFWwindow* wind, int Window_Width, int Window_Height) {
//    //GUI HERE
//    
//}
//
//void Camera(GLFWwindow* wind, int Window_Width, int Window_Height) {
//    //CAMERA STUFF
//}
//
//void Draw(GLFWwindow* wind, int Window_Width, int Window_Height) {
//    //DRAW CALL
//
//}

int main()
{
    tm.Blocking_SearchFor(3);
	tm.PrintToyList();
	tm.GetToy(0)->BlockingRequestConnect();
	tm.GetToy(0)->BlockingRequestStopTracking();
	tm.PrintToyList();
	tm.GetToy(0)->UpdateToyInfo();
	tm.PrintToyList();
	tm.GetToy(0)->BlockingRequestVibrate(1);
	std::this_thread::sleep_for(std::chrono::seconds(5));
	tm.GetToy(0)->BlockingRequestVibrate(0);
	tm.PrintToyList();
}