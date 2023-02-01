// RedPandaEngine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include "Graphics.h"
//#include "Lua.h"
//#include "Python.h"
//#include "Language_Manager.h"
#include "Lovense_Device.h"
#include "Kinect.h"

ToyManager tm;

Graphics::Window window("RedPandaEngine");

float cam_x = 3, cam_y = 3, cam_z = 3;
int level = 0;
int seconds=0;
char buff[255] = { 0 };
char buff2[255]={0};

Kinect::Sensor sensor;
bool DrawGrid = true;
bool VibrateCollision = false;
GLuint TextureID;
GLuint TextureID2;
void* ImageBuffer1;
void* ImageBuffer2;
GLuint frameBuffer;
GLuint renderBuffer;
GLuint texture;

void Update(GLFWwindow* wind, int Window_Width, int Window_Height) {
	//draw kinect camera depth
	sensor.getDepthFrame([](void* data, int size) {
		delete ImageBuffer1;
		ImageBuffer1 = malloc(size);
		memcpy(ImageBuffer1, data, size);
		});
	//bind image to texture id
	glDeleteTextures(1, &TextureID);
	glGenTextures(1, &TextureID);
	glBindTexture(GL_TEXTURE_2D, TextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 640, 480, 0, GL_RGBA, GL_UNSIGNED_BYTE, ImageBuffer1);


	//draw kinect camera color

	glPushMatrix();
	sensor.getColorFrame([](void* data, int size) {
		delete ImageBuffer2;
		ImageBuffer2 = malloc(size);
		memcpy(ImageBuffer2, data, size);
		});
	//render to frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glViewport(0, 0, 1280, 960);
	glClearColor(0.0, 00, 00, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//bind image to texture id
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1280, 960, 0, GL_BGRA, GL_UNSIGNED_BYTE, ImageBuffer2);
	// Draw stuff

	glMatrixMode(GL_PROJECTION_MATRIX);
	glLoadIdentity();
	gluPerspective(45, (float)1280 / (float)960, 0.1, 1000);
	gluLookAt(0,0, -1, 0, 0, 0, 0, 1, 0);
	glTranslated(0, 0, -1);
	glMatrixMode(GL_MODELVIEW_MATRIX);
	
	//get Skeleton and draw
	NUI_SKELETON_FRAME  frame = sensor.getSkeletonFrame();
	for (int i = 0; i < NUI_SKELETON_COUNT; i++)
	{
		glPointSize(10);
		glBegin(GL_POINTS);
		for (int j = 0; j < NUI_SKELETON_POSITION_COUNT; j++)
		{
			glm::vec3 point = glm::vec3(frame.SkeletonData[i].SkeletonPositions[j].x, frame.SkeletonData[i].SkeletonPositions[j].y, frame.SkeletonData[i].SkeletonPositions[j].z);
			Graphics::glColor(Util::HSVtoRGB(360/(i+1), 100, 100));

			glVertex3f(-point.x, -point.y, point.z);
		}
		glEnd();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Window_Width, Window_Height);
	glPopMatrix();
}

//draw functions
void GUI(GLFWwindow* wind, int Window_Width, int Window_Height) {
	if (ImGui::Begin("View")) {
		ImGui::Checkbox("Draw Grid", &DrawGrid);
		ImGui::Checkbox("Vibrate Collision", &VibrateCollision);
	}
	ImGui::End();

	if (ImGui::Begin("Kinect")) {
		//angle
		static float angle = sensor.getAngle();
		ImGui::InputFloat("angle", &angle);
		if (angle != sensor.getAngle()) {
			sensor.setAngle(angle);
		}
		
		ImGui::Image((void*)(intptr_t)TextureID, ImVec2(640,480));

		

		
		ImGui::Image((void*)(intptr_t)TextureID2, ImVec2(1280,960));
		
		//flip uv
		ImGui::Image((void*)texture, ImVec2(1280, 960));

	}
	ImGui::End();

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

void DrawXYZGrid(float from,float to, int resolution,float lineWidth) {
	float dist = to - from;
	float stepSize = dist / resolution;
	//draw x y z grid from -1 to 1
	glLineWidth(lineWidth);
	glBegin(GL_LINES);
	//x grid
	glColor3f(1, 0, 0);
	for (float i = from; i <= to; i += dist / resolution)
	{
		glVertex3f(from, i, 0);
		glVertex3f(to, i, 0);
		glVertex3f(i, from, 0);
		glVertex3f(i, to, 0);
	}
	//y grid
	glColor3f(0, 1, 0);
	for (float i = from; i <= to; i += dist / resolution)
	{
		glVertex3f(from, 0, i);
		glVertex3f(to, 0, i);
		glVertex3f(i, 0, from);
		glVertex3f(i, 0, to);
	}
	//z grid
	glColor3f(0, 0, 1);
	for (float i = from; i <= to; i += dist / resolution)
	{
		glVertex3f(0, from, i);
		glVertex3f(0, to, i);
		glVertex3f(0, i, from);
		glVertex3f(0, i, to);
	}
	glEnd();
}

void Draw(GLFWwindow* wind, int Window_Width, int Window_Height) {
	if(DrawGrid)
	DrawXYZGrid(-1, 1, 4, 2);
	
	//draw cube in top right
	glPushMatrix();
	Graphics::MeshTools::Shapes::Cube cube;
	
    //get Skeleton and draw
	NUI_SKELETON_FRAME  frame = sensor.getSkeletonFrame();
	for (int i = 0; i < NUI_SKELETON_COUNT; i++)
	{
			glPointSize(10);
			glBegin(GL_POINTS);
			for (int j = 0; j < NUI_SKELETON_POSITION_COUNT; j++)
			{
				glm::vec3 point = glm::vec3(frame.SkeletonData[i].SkeletonPositions[j].x, frame.SkeletonData[i].SkeletonPositions[j].y, frame.SkeletonData[i].SkeletonPositions[j].z);
				point -= glm::vec3(0, 0, 2);
				//check if the point is on left side of the screen
				if (cube.Within(point)) {
					//vibrate connected toys
					if (tm.GetToys().size() > 0)
					if (tm.GetToy(0)->GetVibrationLevel2() != atoi(buff)&& VibrateCollision) {
						tm.GetToy(0)->RequestVibrate(atoi(buff),2);
					}
					Graphics::glColor(Util::HSVtoRGB(222, 71, 96));
				}
				else {
					if(tm.GetToys().size()>0)
					if (tm.GetToy(0)->GetVibrationLevel2() != 0 && VibrateCollision) {
						tm.GetToy(0)->RequestVibrate(0,2);
					}
					Graphics::glColor(Util::HSVtoRGB(26, 96, 81));
				}

				glVertex3f(point.x, point.y, point.z);
			}
			glEnd();
	}
	Graphics::glColor(Util::HSVtoRGBA(321, 53, 66,.25));
	cube.Draw();
	glPopMatrix();
}

int main()
{
	
	sensor.setAngle(10);
	sensor.setAngle(0);
	Graphics::SetCallBackWindow(&window);
	window.Set_Update_function(Update);
	window.Set_GUI_function(GUI);
	window.Set_Camera_function(Camera);
	window.Set_Draw_function(Draw);
	window.Init();
	
	//setup frame buffer, render buffer, and texture
	glGenFramebuffers(1, &frameBuffer);
	glGenTextures(1, &texture);
	glGenRenderbuffers(1, &renderBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1280, 960, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

	
	glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1280, 960);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);

	//check if frame buffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "frame buffer not complete" << std::endl;
	}
	//unbind frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	//render to screen	
	window.Loop();
	window.CleanUp();
}