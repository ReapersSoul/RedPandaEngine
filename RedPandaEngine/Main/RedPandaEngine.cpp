// RedPandaEngine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <filesystem>

#define STB_IMAGE_IMPLEMENTATION
#include <Graphics/Graphics.h>
// #include "Lua.h"
// #include "Python.h"
// #include "Language_Manager.h"
// #include <Devices/Lovense/Lovense_Device.h>
// #include <Devices/Kinect/Kinect.h>

#include <Physics/Physics.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// ToyManager tm;

Graphics::Window window("RedPandaEngine");

bool InvertX = false;
bool InvertY = false;
float cam_R = 3, cam_Angle_X = .0001, cam_Angle_Y = .0001, speed = .01, rotspeed = .005;
float cam_X = 0, cam_Y = 0, cam_Z = 0;
float camk[3] = {0};
int level = 0;
int seconds = 0;
char buff[255] = {0};
char buff2[255] = {0};

// Kinect::Sensor sensor;
bool DrawGrid = true;
// bool VibrateCollision = false;
// bool VibeController = false;
// GLuint TextureID;
// GLuint TextureID2;
// void* ImageBuffer1;
// void* ImageBuffer2;
// GLuint frameBuffer;
// GLuint renderBuffer;
// GLuint texture;
float color = 0;
Graphics::MeshTools::Shapes::Cube cube;
Physics::World world;
float Deadzone = 0.1f;
// Kinect::Skeleton skeletons[NUI_SKELETON_COUNT];

int joystick = -1;

Assimp::Importer importer;
const aiScene *scene;
std::vector<GLuint> textures;

void DrawModel()
{
	// load base.dae model using assimp and draw
	if (!scene)
	{
		scene = importer.ReadFile("../../RedPandaEngine/Assets/base2.dae", aiProcess_Triangulate | aiProcess_FlipUVs);
		if (!scene)
		{
			PLOGD_(Util::Logs::Error) << "Failed to load model! Current path: " << std::filesystem::current_path() << " asset path: " << std::filesystem::absolute("../../RedPandaEngine/Assets/base.dae");
		}
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			// load textures
			aiString path;
			scene->mMaterials[scene->mMeshes[i]->mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, 0, &path);
			std::string texturePath = "../../RedPandaEngine/Assets/" + std::string(path.C_Str());
			GLuint texture = Graphics::LoadTexture(texturePath.c_str());
			textures.push_back(texture);
		}
	}
	else
	{
		// draw model
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			glEnable(GL_TEXTURE_2D);
			glPushMatrix();
			glRotatef(-90, 1, 0, 0);
			glBindTexture(GL_TEXTURE_2D, textures[i]);
			// draw mesh
			const aiMesh *mesh = scene->mMeshes[i];
			for (int j = 0; j < mesh->mNumFaces; j++)
			{
				const aiFace *face = &mesh->mFaces[j];
				glBegin(GL_TRIANGLES);
				for (int k = 0; k < 3; k++)
				{
					int index = face->mIndices[k];
					glColor3f(1, 1, 1);
					glNormal3f(mesh->mNormals[index].x, mesh->mNormals[index].y, mesh->mNormals[index].z);
					glTexCoord2f(mesh->mTextureCoords[0][index].x, mesh->mTextureCoords[0][index].y);
					glVertex3f(mesh->mVertices[index].x, mesh->mVertices[index].y, mesh->mVertices[index].z);
				}
				glEnd();
			}
			glBindTexture(GL_TEXTURE_2D, 0);
			glPopMatrix();
			glDisable(GL_TEXTURE_2D);
		}
	}
}

void Update(GLFWwindow *wind, int Window_Width, int Window_Height)
{
	if (joystick != -1)
	{
		if (glfwJoystickPresent(joystick))
		{
			int count;
			const float *axis = glfwGetJoystickAxes(joystick, &count);
			// if (VibeController) {
			// 	if (tm.GetToys().size() > 0) {
			// 		if (tm.GetToy(0)->GetConnected()) {
			// 			int vibelevel = floor(Util::map<float>(axis[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER], -1, 1, 0, 20));
			// 			if (vibelevel != tm.GetToy(0)->GetVibrationLevel()) {
			// 				tm.GetToy(0)->RequestVibrate(vibelevel);
			// 			}
			// 		}
			// 	}
			// }

			// use left stick to move camera position based on where cam is looking
			// check deadzone
			if (axis[GLFW_GAMEPAD_AXIS_LEFT_Y] > Deadzone)
			{
				glm::vec3 camPosTranslation = Util::PointOnSphere(1, cam_Angle_X, cam_Angle_Y) * speed * axis[GLFW_GAMEPAD_AXIS_LEFT_Y];
				cam_X += camPosTranslation.x;
				cam_Y += camPosTranslation.y;
				cam_Z += camPosTranslation.z;
			}
			else if (axis[GLFW_GAMEPAD_AXIS_LEFT_Y] < -Deadzone)
			{
				glm::vec3 camPosTranslation = Util::PointOnSphere(1, cam_Angle_X, cam_Angle_Y) * speed * -axis[GLFW_GAMEPAD_AXIS_LEFT_Y];
				cam_X -= camPosTranslation.x;
				cam_Y -= camPosTranslation.y;
				cam_Z -= camPosTranslation.z;
			}
			else if (axis[GLFW_GAMEPAD_AXIS_LEFT_X] > Deadzone)
			{
				// take cross product of camera position and up vector to get right vector
				glm::vec3 right = glm::cross(Util::PointOnSphere(1, cam_Angle_X, cam_Angle_Y), glm::vec3(0, 1, 0));
				glm::vec3 camPosTranslation = right * speed * axis[GLFW_GAMEPAD_AXIS_LEFT_X];
				cam_X -= camPosTranslation.x;
				cam_Y -= camPosTranslation.y;
				cam_Z -= camPosTranslation.z;
			}
			else if (axis[GLFW_GAMEPAD_AXIS_LEFT_X] < -Deadzone)
			{
				// take cross product of camera position and up vector to get right vector
				glm::vec3 right = glm::cross(Util::PointOnSphere(1, cam_Angle_X, cam_Angle_Y), glm::vec3(0, 1, 0));
				glm::vec3 camPosTranslation = right * speed * -axis[GLFW_GAMEPAD_AXIS_LEFT_X];
				cam_X += camPosTranslation.x;
				cam_Y += camPosTranslation.y;
				cam_Z += camPosTranslation.z;
			}

			// if rt is pressed shrink camera radius
			if (axis[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER])
			{
				cam_R += speed * axis[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER];
			}
			// if lt is pressed grow camera radius
			if (axis[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER])
			{
				cam_R -= speed * axis[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER];
			}

			if (cam_R < 0)
			{
				cam_R = 0;
			}

			// if rb is pressed move camera up
			if (glfwGetJoystickButtons(joystick, &count)[7])
			{
				cam_Y += speed;
			}
			// if lb is pressed move camera down
			if (glfwGetJoystickButtons(joystick, &count)[6])
			{
				cam_Y -= speed;
			}

			// for every button if its pressed print it to the log
			for (int i = 0; i < 15; i++)
			{
				static bool pressed[15] = {false};
				if (glfwGetJoystickButtons(joystick, &count)[i] && !pressed[i])
				{
					PLOGD_(Util::Logs::Debug) << "Button " << i << " pressed";
					pressed[i] = true;
				}
				else if (!glfwGetJoystickButtons(joystick, &count)[i] && pressed[i])
				{
					pressed[i] = false;
				}
			}
		}
	}

	// if (sensor.IsInit()) {
	// 	//draw kinect camera depth
	// 	sensor.getDepthFrame([](void* data, int size) {
	// 		delete ImageBuffer1;
	// 		ImageBuffer1 = malloc(size);
	// 		memcpy(ImageBuffer1, data, size);
	// 		});
	// 	//bind image to texture id
	// 	glDeleteTextures(1, &TextureID);
	// 	glGenTextures(1, &TextureID);
	// 	glBindTexture(GL_TEXTURE_2D, TextureID);
	// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// 	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 640, 480, 0, GL_RGBA, GL_UNSIGNED_BYTE, ImageBuffer1);

	// 	//draw kinect camera color
	// 	glPushMatrix();
	// 	//sensor.getColorFrame([](void* data, int size) {
	// 	//	delete ImageBuffer2;
	// 	//	ImageBuffer2 = malloc(size);
	// 	//	memcpy(ImageBuffer2, data, size);
	// 	//	});
	// 	//render to frame buffer
	// 	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	// 	glViewport(0, 0, 1280, 960);
	// 	glClearColor(0.0, 00, 00, 1.0);
	// 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// 	//bind image to texture id
	// 	glBindTexture(GL_TEXTURE_2D, texture);
	// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// 	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1280, 960, 0, GL_BGRA, GL_UNSIGNED_BYTE, ImageBuffer2);
	// 	// Draw stuff

	// 	glMatrixMode(GL_PROJECTION_MATRIX);
	// 	glLoadIdentity();
	// 	gluPerspective(45, (float)1280 / (float)960, 0.1, 1000);
	// 	gluLookAt(camk[0], camk[1], camk[2], 0, 0, 0, 0, 1, 0);
	// 	//glTranslated(-1, 0, 0);
	// 	glMatrixMode(GL_MODELVIEW_MATRIX);

	// 	//get Skeleton and draw
	// 	NUI_SKELETON_FRAME  frame = sensor.getSkeletonFrame();
	// 	for (int i = 0; i < NUI_SKELETON_COUNT; i++)
	// 	{
	// 		if (frame.SkeletonData[i].eTrackingState == NUI_SKELETON_TRACKED) {
	// 			skeletons[i].Update(frame.SkeletonData[i]);
	// 		}
	// 		skeletons->DrawJoints(10, [&i](glm::vec4 p) {
	// 			Graphics::glColor(Util::HSVtoRGB(360 / (i + 1), 100, 100));
	// 			return p;
	// 			});
	// 	}
	// }
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Window_Width, Window_Height);
	glPopMatrix();
}

// draw functions
void GUI(GLFWwindow *wind, int Window_Width, int Window_Height)
{
	// dock over viewport make suer its see through
	// ImGui::DockSpaceOverViewport();

	if (ImGui::Begin("Log"))
	{
		// tabs of logs
		if (ImGui::BeginTabBar("Logs"))
		{
			if (ImGui::BeginTabItem("Info"))
			{
				for (int i = 0; i < Util::Logs::InfoLog.getMessageList().size(); i++)
				{
					ImGui::TextUnformatted(Util::Logs::InfoLog.getMessageList()[i].c_str());
				}
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Errors"))
			{
				for (int i = 0; i < Util::Logs::ErrorLog.getMessageList().size(); i++)
				{
					ImGui::TextUnformatted(Util::Logs::ErrorLog.getMessageList()[i].c_str());
				}
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Debug"))
			{
				for (int i = 0; i < Util::Logs::DebugLog.getMessageList().size(); i++)
				{
					ImGui::TextUnformatted(Util::Logs::DebugLog.getMessageList()[i].c_str());
				}

				ImGui::EndTabItem();
			}
			// if (ImGui::BeginTabItem("Lovense")) {
			// 	for (int i = 0; i < LovenseLog.getMessageList().size(); i++)
			// 	{
			// 		ImGui::TextUnformatted(LovenseLog.getMessageList()[i].c_str());
			// 	}
			// 	ImGui::EndTabItem();
			// }
			// if (ImGui::BeginTabItem("Kinect")) {
			// 	for (int i = 0; i < KinectLog.getMessageList().size(); i++)
			// 	{
			// 		ImGui::TextUnformatted(KinectLog.getMessageList()[i].c_str());
			// 	}
			// 	ImGui::EndTabItem();
			// }
			ImGui::EndTabBar();
		}
	}
	ImGui::End();

	if (ImGui::Begin("View"))
	{
		ImGui::Checkbox("Draw Grid", &DrawGrid);

		// //radio
		// static int ViewMode = 2;
		// ImGui::RadioButton("None", &ViewMode, 2);
		// ImGui::RadioButton("Vibrate Collision", &ViewMode, 0);
		// ImGui::RadioButton("Vibrate Controller", &ViewMode, 1);
		// switch (ViewMode)
		// {
		// case 0:
		// 	//vibrate collision
		// 	VibrateCollision = true;
		// 	VibeController = false;
		// 	break;
		// case 1:
		// 	//vibrate controller
		// 	VibrateCollision = false;
		// 	VibeController = true;
		// 	break;
		// default:
		// 	break;
		// }
	}
	ImGui::End();

	// if (ImGui::Begin("Kinect")) {
	// 	if (sensor.IsInit()) {
	// 		//angle
	// 		static float angle = sensor.getAngle();
	// 		ImGui::InputFloat("angle", &angle);
	// 		if (angle != sensor.getAngle()) {
	// 			sensor.setAngle(angle);
	// 		}

	// 		ImGui::Image((void*)(intptr_t)TextureID, ImVec2(640, 480));
	// 		ImGui::DragFloat3("XYZ", camk, .1);
	// 		ImGui::Image((void*)texture, ImVec2(1280, 960));
	// 	}
	// }
	// ImGui::End();

	// GUI HERE
	//  if (ImGui::Begin("Toy Manager")) {
	//  	if (ImGui::Button("Search")) {
	//  		tm.NonBlocking_SearchFor(5);
	//  	}
	//  	std::map<std::string, Toy*> toys = tm.GetToys();
	//  	ImGui::BeginTable("Toys", 9);
	//  	ImGui::TableSetupColumn("Toy ID");
	//  	ImGui::TableSetupColumn("Toy Name");
	//  	ImGui::TableSetupColumn("Battery Level");
	//  	ImGui::TableSetupColumn("Conected Status");
	//  	ImGui::TableHeadersRow();

	// 	for (auto it=toys.begin(); it != toys.end(); it++)
	// 	{
	// 		ImGui::TableNextRow();
	// 		ImGui::TableNextColumn();
	// 		ImGui::Text(it->second->GetID().c_str());
	// 		ImGui::TableNextColumn();
	// 		ImGui::Text(it->second->GetName().c_str());
	// 		ImGui::TableNextColumn();
	// 		ImGui::Text(std::to_string(it->second->GetBatteryLevel()).c_str());
	// 		ImGui::TableNextColumn();
	// 		ImGui::Text(std::to_string(it->second->GetConnected()).c_str());
	// 		ImGui::TableNextColumn();
	// 		if (ImGui::Button("Connect")) {
	// 			it->second->RequestConnect();
	// 			it->second->RequestBattery();
	// 		}
	// 		ImGui::TableNextColumn();
	// 		if (ImGui::Button("Disconnect"))
	// 			it->second->RequestDisConnect();
	// 		ImGui::TableNextColumn();
	// 		if (ImGui::Button("Vibrate")) {
	// 			it->second->RequestVibrate(level);
	// 			std::string id= it->second->GetID();
	// 			std::thread([id]() {
	// 				std::this_thread::sleep_for(std::chrono::seconds(seconds));
	// 				tm.GetToy(id)->RequestVibrate(0);
	// 				}).detach();
	// 		}
	// 		ImGui::TableNextColumn();
	// 		ImGui::InputText("Level", buff, 255);
	// 		level = atoi(buff);
	// 		ImGui::TableNextColumn();
	// 		ImGui::InputText("Seconds", buff2, 255);
	// 		seconds = atoi(buff2);
	// 	}
	// 	ImGui::EndTable();
	// }
	// ImGui::End();
	if (ImGui::Begin("Camera"))
	{
		ImGui::InputInt("Joystick", &joystick);
		ImGui::DragFloat("Deadzone", &Deadzone, 0.01f);
		ImGui::Checkbox("Invert X", &InvertX);
		ImGui::Checkbox("Invert Y", &InvertY);
		ImGui::DragFloat("Camera Radius", &cam_R, 0.1f);
		ImGui::DragFloat("Camera Speed", &speed, 0.1f);
		ImGui::DragFloat("Camera Rotation Speed", &rotspeed, 0.1f);
		ImGui::DragFloat("Camera X", &cam_X, 0.1f);
		ImGui::DragFloat("Camera Y", &cam_Y, 0.1f);
		ImGui::DragFloat("Camera Z", &cam_Z, 0.1f);
	}
	ImGui::End();
}

void Camera(GLFWwindow *wind, int Window_Width, int Window_Height)
{
	// CAMERA STUFF
	gluPerspective(45, (float)Window_Width / (float)Window_Height, 0.1, 1000);
	if (joystick != -1)
	{
		if (glfwJoystickPresent(joystick))
		{
			float v_invertY = 1;
			float v_invertX = 1;
			if (InvertX)
			{
				v_invertX = -1;
			}
			if (InvertY)
			{
				v_invertY = -1;
			}
			// get right stick
			int count = 0;
			const float *axis = glfwGetJoystickAxes(joystick, &count);
			if (axis[GLFW_GAMEPAD_AXIS_RIGHT_X] > .1 || axis[GLFW_GAMEPAD_AXIS_RIGHT_X] < -.1 || axis[GLFW_GAMEPAD_AXIS_RIGHT_Y] > .1 || axis[GLFW_GAMEPAD_AXIS_RIGHT_Y] < -.1)
			{
				cam_Angle_X += Util::map<float>(axis[GLFW_GAMEPAD_AXIS_RIGHT_X], -1, 1, -M_PI, M_PI) * rotspeed * v_invertX;
				cam_Angle_Y += Util::map<float>(axis[GLFW_GAMEPAD_AXIS_RIGHT_Y], -1, 1, M_PI, -M_PI) * rotspeed * v_invertY;
				// if Camera Angle Y is greater than 90 degrees limit it to 90 degrees
				if (cam_Angle_Y > -0.001)
				{
					cam_Angle_Y = -0.001;
				}
				// if Camera Angle Y is less than -90 degrees limit it to -90 degrees
				if (cam_Angle_Y < -M_PI + 0.001)
				{
					cam_Angle_Y = -M_PI + 0.001;
				}
			}
		}
	}
	glm::vec3 campos = Util::PointOnSphere(cam_R, cam_Angle_X, cam_Angle_Y) + glm::vec3(cam_X, cam_Y, cam_Z);
	gluLookAt(campos.x, campos.y, campos.z, cam_X, cam_Y, cam_Z, 0, 1, 0);
}

void DrawXYZGrid(float from, float to, int resolution, float lineWidth)
{
	float dist = to - from;
	float stepSize = dist / resolution;
	// draw x y z grid from -1 to 1
	glLineWidth(lineWidth);
	glBegin(GL_LINES);
	// x grid
	glColor3f(1, 0, 0);
	for (float i = from; i <= to; i += dist / resolution)
	{
		glVertex3f(from, i, 0);
		glVertex3f(to, i, 0);
		glVertex3f(i, from, 0);
		glVertex3f(i, to, 0);
	}
	// y grid
	glColor3f(0, 1, 0);
	for (float i = from; i <= to; i += dist / resolution)
	{
		glVertex3f(from, 0, i);
		glVertex3f(to, 0, i);
		glVertex3f(i, 0, from);
		glVertex3f(i, 0, to);
	}
	// z grid
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

void Draw(GLFWwindow *wind, int Window_Width, int Window_Height)
{
	if (DrawGrid)
		DrawXYZGrid(-1, 1, 4, 2);

	// draw cube in top right
	glPushMatrix();

	// if (sensor.IsInit()) {
	// 	//get Skeleton and draw
	// 	for (int i = 0; i < NUI_SKELETON_COUNT; i++)
	// 	{
	// 		skeletons[i].DrawAppliedJoints(10, [](glm::vec4 point) {
	// 			Graphics::glColor(Util::HSVtoRGBA(360, 0, 100, 1));
	// 			return point;
	// 			});
	// 	}
	// }

	DrawModel();

	Graphics::glColor(Util::HSVtoRGBA(color, 53, 66, .25));
	cube.Draw();

	glPopMatrix();
}

// void InitFrameBuffer() {
// 	//setup frame buffer, render buffer, and texture
// 	glGenFramebuffers(1, &frameBuffer);
// 	glGenTextures(1, &texture);
// 	glGenRenderbuffers(1, &renderBuffer);
// 	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

// 	glBindTexture(GL_TEXTURE_2D, texture);
// 	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1280, 960, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
// 	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

// 	glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
// 	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1280, 960);
// 	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);

// 	//check if frame buffer is complete
// 	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
// 	{
// 		std::cout << "frame buffer not complete" << std::endl;
// 	}
// 	//unbind frame buffer
// 	glBindFramebuffer(GL_FRAMEBUFFER, 0);
// }

// create main event proceassor
class MainEventProcessor : public EventStream::EventProcessor
{
	bool HandleEvent(EventStream::Event *e) override
	{
		std::string Type = e->EventType;
		if (Type == "JoystickEvent")
		{
			EventStream::JoystickEvent *je = (EventStream::JoystickEvent *)e;
			if (glfwJoystickIsGamepad(je->jid))
			{
				PLOGD_(Util::Logs::Debug) << "Joy";
			}
		}
		return true;
	};
};

int main()
{

	// for (int i = 0; i < NUI_SKELETON_COUNT; i++)
	// {
	// 		for (int j = 0; j < NUI_SKELETON_POSITION_COUNT; j++) {
	// 			Physics::LinkedCollisionPointVec4* p = new Physics::LinkedCollisionPointVec4(skeletons[i].GetAppliedJointRef(j));
	// 			p->onEnterCollision = [i](Physics::CollisionObject* main, Physics::CollisionObject* other) {
	// 				if (skeletons[i].IsTracked() && other->GetName() == "BOX")
	// 					PLOGD_(Util::Logs::Debug) << "Entered Collision!";
	// 			};
	// 			p->onExitCollision = [i](Physics::CollisionObject* main, Physics::CollisionObject* other) {
	// 				if (skeletons[i].IsTracked() && other->GetName() == "BOX")
	// 					PLOGD_(Util::Logs::Debug) << "Exited Collision!";
	// 			};
	// 			p->onCollision = [i](Physics::CollisionObject* main, Physics::CollisionObject* other) {
	// 				//if (skeletons[i].IsTracked() && other->GetName()=="BOX")
	// 					//PLOGD_(Util::Logs::Debug) << "Collision!";
	// 			};
	// 			world.Add(p);
	// 		}
	// 		skeletons[i].SetPosition({ 0,0,-2 });
	// 		skeletons[i].SetScale({ 1,1,1 });
	// }
	world.Add(new Physics::CollisionBox({0, 0, 0}, {1, 1, 1}));

	// if (sensor.Init()) {
	// 	sensor.setAngle(10);
	// 	sensor.setAngle(0);
	// }
	Graphics::SetCallBackWindow(&window);
	window.AddEventProcessor(new MainEventProcessor());
	window.Set_Update_function(Update);
	window.Set_GUI_function(GUI);
	window.Set_Camera_function(Camera);
	window.Set_Draw_function(Draw);
	window.Init();

	// InitFrameBuffer();

	Util::Logs::InitLogs();
	// InitLovenseLog();
	// InitKinectLog();
	// render to screen
	window.Loop();
	window.CleanUp();
}