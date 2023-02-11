// RedPandaEngine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "Graphics.h"
//#include "Lua.h"
//#include "Python.h"
//#include "Language_Manager.h"
#include "Lovense_Device.h"
#include "Physics.h"
#include "Kinect.h"
#include "util.h"

ToyManager tm;

Graphics::Window window("RedPandaEngine");

float cam_R = 3, cam_Angle_X = 0, cam_Angle_Y = 0, speed = 1, rotspeed = .1;
float cam_X = 0, cam_Y = 0, cam_Z = 0;
float camk[3]={0};
int level = 0;
int seconds=0;
char buff[255] = { 0 };
char buff2[255]={0};

Kinect::Sensor sensor;
bool DrawGrid = true;
bool VibrateCollision = false;
bool VibeController = false;
GLuint TextureID;
GLuint TextureID2;
void* ImageBuffer1;
void* ImageBuffer2;
GLuint frameBuffer;
GLuint renderBuffer;
GLuint texture;
float color = 0;
Graphics::MeshTools::Shapes::Cube cube;
Physics::World world;

Kinect::Skeleton skeletons[NUI_SKELETON_COUNT];

void Update(GLFWwindow* wind, int Window_Width, int Window_Height) {
	if (glfwJoystickPresent(GLFW_JOYSTICK_1)) {
		int count;
		const float* axis = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count);
		color = Util::map<float>(axis[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER], -1, 1, 0, 360);
		if (VibeController) {
			if (tm.GetToys().size() > 0) {
				if (tm.GetToy(0)->GetConnected()) {
					int vibelevel = floor(Util::map<float>(axis[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER], -1, 1, 0, 20));
					if (vibelevel != tm.GetToy(0)->GetVibrationLevel()) {
						tm.GetToy(0)->RequestVibrate(vibelevel);
					}
				}
			}
		}
		glm::vec3 camPos=glm::vec3(0);
		//use left stick to move camera position based on where cam is looking
		//check deadzone
		if (axis[GLFW_GAMEPAD_AXIS_LEFT_Y] > 0.1 || axis[GLFW_GAMEPAD_AXIS_LEFT_Y] < -0.1|| axis[GLFW_GAMEPAD_AXIS_LEFT_X] > 0.1 || axis[GLFW_GAMEPAD_AXIS_LEFT_X] < -0.) {
			camPos = Util::PointOnSphere(1, cam_Angle_X, cam_Angle_Y) * glm::vec3(axis[GLFW_GAMEPAD_AXIS_LEFT_Y])*speed;
			float tmp = camPos.x;
			camPos.x = camPos.y;
			camPos.y = tmp;
		}
		cam_X += camPos.x;
		cam_Y += camPos.y;
		cam_Z += camPos.z;
		
		
	}
	
	if (sensor.IsInit()) {
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
		//sensor.getColorFrame([](void* data, int size) {
		//	delete ImageBuffer2;
		//	ImageBuffer2 = malloc(size);
		//	memcpy(ImageBuffer2, data, size);
		//	});
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
		gluLookAt(camk[0], camk[1], camk[2], 0, 0, 0, 0, 1, 0);
		//glTranslated(-1, 0, 0);
		glMatrixMode(GL_MODELVIEW_MATRIX);

		//get Skeleton and draw
		NUI_SKELETON_FRAME  frame = sensor.getSkeletonFrame();
		for (int i = 0; i < NUI_SKELETON_COUNT; i++)
		{
			if (frame.SkeletonData[i].eTrackingState == NUI_SKELETON_TRACKED) {
				skeletons[i].Update(frame.SkeletonData[i]);
			}
			skeletons->DrawJoints(10, [&i](glm::vec4 p) {
				Graphics::glColor(Util::HSVtoRGB(360 / (i + 1), 100, 100));
				return p;
				});
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Window_Width, Window_Height);
	glPopMatrix();
}

//draw functions
void GUI(GLFWwindow* wind, int Window_Width, int Window_Height) {
	if (ImGui::Begin("Log")) {
		//tabs of logs
		if (ImGui::BeginTabBar("Logs")) {
			if (ImGui::BeginTabItem("Info")) {
				for (int i = 0; i < Util::Logs::InfoLog.getMessageList().size(); i++)
				{
					ImGui::TextUnformatted(Util::Logs::InfoLog.getMessageList()[i].c_str());
				}
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Errors")) {
				for (int i = 0; i < Util::Logs::ErrorLog.getMessageList().size(); i++)
				{
					ImGui::TextUnformatted(Util::Logs::ErrorLog.getMessageList()[i].c_str());
				}
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Debug")) {
				for (int i = 0; i < Util::Logs::DebugLog.getMessageList().size(); i++)
				{
					ImGui::TextUnformatted(Util::Logs::DebugLog.getMessageList()[i].c_str());
				}

				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Lovense")) {
				for (int i = 0; i < LovenseLog.getMessageList().size(); i++)
				{
					ImGui::TextUnformatted(LovenseLog.getMessageList()[i].c_str());
				}
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Kinect")) {
				for (int i = 0; i < KinectLog.getMessageList().size(); i++)
				{
					ImGui::TextUnformatted(KinectLog.getMessageList()[i].c_str());
				}
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
	}
	ImGui::End();
	
	if (ImGui::Begin("View")) {
		ImGui::Checkbox("Draw Grid", &DrawGrid);

		//radio
		static int ViewMode = 2;
		ImGui::RadioButton("None", &ViewMode, 2);
		ImGui::RadioButton("Vibrate Collision", &ViewMode, 0);
		ImGui::RadioButton("Vibrate Controller", &ViewMode, 1);
		switch (ViewMode)
		{
		case 0:
			//vibrate collision
			VibrateCollision = true;
			VibeController = false;
			break;
		case 1:
			//vibrate controller
			VibrateCollision = false;
			VibeController = true;
			break;
		default:
			break;
		}
	}
	ImGui::End();

	if (ImGui::Begin("Kinect")) {
		if (sensor.IsInit()) {
			//angle
			static float angle = sensor.getAngle();
			ImGui::InputFloat("angle", &angle);
			if (angle != sensor.getAngle()) {
				sensor.setAngle(angle);
			}

			ImGui::Image((void*)(intptr_t)TextureID, ImVec2(640, 480));
			ImGui::DragFloat3("XYZ", camk, .1);
			ImGui::Image((void*)texture, ImVec2(1280, 960));
		}
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
		ImGui::TableSetupColumn("Battery Level");
		ImGui::TableSetupColumn("Conected Status");
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
			if (ImGui::Button("Connect")) {
				it->second->RequestConnect();
				it->second->RequestBattery();
			}
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
		ImGui::DragFloat("Camera Radius", &cam_R, 0.1f);
		ImGui::DragFloat("Camera Speed", &speed, 0.1f);
		ImGui::DragFloat("Camera Rotation Speed", &rotspeed, 0.1f);
	}
	ImGui::End();
}

void Camera(GLFWwindow* wind, int Window_Width, int Window_Height) {
    //CAMERA STUFF
	gluPerspective(45, (float)Window_Width / (float)Window_Height, 0.1, 1000);
	if (glfwJoystickPresent(GLFW_JOYSTICK_1)) {
		//get right stick
		int count = 0;
		const float* axis = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count);
		if (axis[GLFW_GAMEPAD_AXIS_RIGHT_X] > .1 || axis[GLFW_GAMEPAD_AXIS_RIGHT_X] < -.1|| axis[GLFW_GAMEPAD_AXIS_RIGHT_Y] > .1 || axis[GLFW_GAMEPAD_AXIS_RIGHT_Y] < -.1){
			cam_Angle_X += Util::map<float>(axis[GLFW_GAMEPAD_AXIS_RIGHT_X], -1, 1, -M_PI, M_PI)*rotspeed;
			cam_Angle_Y += Util::map<float>(axis[GLFW_GAMEPAD_AXIS_RIGHT_Y], -1, 1, M_PI, -M_PI)*rotspeed;
		}
	}
	glm::vec3 campos = Util::PointOnSphere(cam_R, cam_Angle_X, cam_Angle_Y) + glm::vec3(cam_X, cam_Y, cam_Z);
	gluLookAt(campos.x, campos.y, campos.z, cam_X, cam_Y, cam_Z, 0, 1, 0);
}

void DrawXYZGrid(float from, float to, int resolution, float lineWidth) {
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
	if (DrawGrid)
		DrawXYZGrid(-1, 1, 4, 2);

	//draw cube in top right
	glPushMatrix();

	if (sensor.IsInit()) {
		//get Skeleton and draw
		for (int i = 0; i < NUI_SKELETON_COUNT; i++)
		{
			skeletons[i].DrawAppliedJoints(10, [](glm::vec4 point) {
				Graphics::glColor(Util::HSVtoRGBA(360, 0, 100, 1));
				return point;
				});
		}
	}
	Graphics::glColor(Util::HSVtoRGBA(color, 53, 66, .25));
	cube.Draw();
	glPopMatrix();
}

void InitFrameBuffer() {
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
}

//create main event proceassor
class MainEventProcessor : public EventStream::EventProcessor {
	bool HandleEvent(EventStream::Event* e) override {
		std::string Type = e->EventType;
		if (Type == "JoystickEvent") {
			EventStream::JoystickEvent* je = (EventStream::JoystickEvent*)e;
			if (glfwJoystickIsGamepad(je->jid)) {
				PLOGD_(Util::Logs::Debug) << "Joy";
			}
		}
		return true;
	};
};

int main()
{
	::ShowWindow(::GetConsoleWindow(), SW_HIDE);

	for (int i = 0; i < NUI_SKELETON_COUNT; i++)
	{
			for (int j = 0; j < NUI_SKELETON_POSITION_COUNT; j++) {
				Physics::LinkedCollisionPointVec4* p = new Physics::LinkedCollisionPointVec4(skeletons[i].GetAppliedJointRef(j));
				p->onEnterCollision = [i](Physics::CollisionObject* main, Physics::CollisionObject* other) {
					if (skeletons[i].IsTracked() && other->GetName() == "BOX")
						PLOGD_(Util::Logs::Debug) << "Entered Collision!";
				};
				p->onExitCollision = [i](Physics::CollisionObject* main, Physics::CollisionObject* other) {
					if (skeletons[i].IsTracked() && other->GetName() == "BOX")
						PLOGD_(Util::Logs::Debug) << "Exited Collision!";
				};
				p->onCollision = [i](Physics::CollisionObject* main, Physics::CollisionObject* other) {
					//if (skeletons[i].IsTracked() && other->GetName()=="BOX")
						//PLOGD_(Util::Logs::Debug) << "Collision!";
				};
				world.Add(p);
			}
			skeletons[i].SetPosition({ 0,0,-2 });
			skeletons[i].SetScale({ 1,1,1 });
	}
	world.Add(new Physics::CollisionBox({0,0,0}, {1,1,1}));

	if (sensor.Init()) {
		sensor.setAngle(10);
		sensor.setAngle(0);
	}
	Graphics::SetCallBackWindow(&window);
	window.AddEventProcessor(new MainEventProcessor());
	window.Set_Update_function(Update);
	window.Set_GUI_function(GUI);
	window.Set_Camera_function(Camera);
	window.Set_Draw_function(Draw);
	window.Init();
	
	InitFrameBuffer();
	
	Util::Logs::InitLogs();
	InitLovenseLog();
	InitKinectLog();
	//render to screen	
	window.Loop();
	window.CleanUp();
}