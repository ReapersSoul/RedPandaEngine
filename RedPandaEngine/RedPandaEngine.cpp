// RedPandaEngine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include "Graphics.h"
//#include "Lua.h"
//#include "Python.h"
//#include "Language_Manager.h"
#include "Lovense_Device.h"

ToyManager tm;

Graphics::Window window("RedPandaEngine");

float cam_x = 0, cam_y = 0, cam_z = 0;
int level = 0;
int seconds=0;
char buff[255] = { 0 };
char buff2[255]={0};

//draw functions
void GUI(GLFWwindow* wind, int Window_Width, int Window_Height) {
    //GUI HERE
	if (ImGui::Begin("Toy Manager")) {
		if (ImGui::Button("Search")) {
			tm.NonBlocking_SearchFor(5);
		}
		std::map<std::string, Toy*> toys = tm.GetToys();
		ImGui::BeginTable("Toys", 9);
		ImGui::TableSetupColumn("Toy ID");
		ImGui::TableSetupColumn("Toy Name");
		ImGui::TableHeadersRow();
				
		for (auto it=toys.begin(); it != toys.end(); it++)
		{
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text(it->second->GetID().c_str());
			ImGui::TableNextColumn();
			ImGui::Text(it->second->GetName().c_str());
			ImGui::TableNextColumn();
			ImGui::Text(std::to_string(it->second->GetBatteryLevel()).c_str());
			ImGui::TableNextColumn();
			ImGui::Text(std::to_string(it->second->GetConnected()).c_str());
			ImGui::TableNextColumn();
			if (ImGui::Button("Connect"))
				it->second->RequestConnect();
			ImGui::TableNextColumn();
			if (ImGui::Button("Disconnect"))
				it->second->RequestDisConnect();
			ImGui::TableNextColumn();
			if (ImGui::Button("Vibrate")) {
				it->second->RequestVibrate(level);
				std::string id= it->second->GetID();
				std::thread([id]() {
					std::this_thread::sleep_for(std::chrono::seconds(seconds));
					tm.GetToy(id)->RequestVibrate(0);
					}).detach();
			}
			ImGui::TableNextColumn();
			ImGui::InputText("Level", buff, 255);
			level = atoi(buff);
			ImGui::TableNextColumn();
			ImGui::InputText("Seconds", buff2, 255);
			seconds = atoi(buff2);
		}
		ImGui::EndTable();
	}
	ImGui::End();
	if (ImGui::Begin("Camera")) {
		ImGui::DragFloat("Camera X", &cam_x, 0.1f);
		ImGui::DragFloat("Camera Y", &cam_y, 0.1f);
		ImGui::DragFloat("Camera Z", &cam_z, 0.1f);
	}
	ImGui::End();
}

void Camera(GLFWwindow* wind, int Window_Width, int Window_Height) {
    //CAMERA STUFF
	gluPerspective(45, (float)Window_Width / (float)Window_Height, 0.1, 1000);
	gluLookAt(cam_x, cam_y, cam_z, 0, 0, 0, 0, 1, 0);
}

void Draw(GLFWwindow* wind, int Window_Width, int Window_Height) {
    //DRAW CALL
	Graphics::MeshTools::Shapes::Cube quad;
	//pink
	glColor3d(1, .65, .65);
	glScaled(.5, .5, .5);
	quad.Draw();
}

int main()
{
	Graphics::SetCallBackWindow(&window);
	window.Set_GUI_function(GUI);
	window.Set_Camera_function(Camera);
	window.Set_Draw_function(Draw);
	window.Init();
	window.Loop();
	window.CleanUp();
}