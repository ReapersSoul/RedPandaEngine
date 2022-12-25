#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <string>
#include <sstream>
#include <thread>
#ifndef FREE_NEHE_H
#define FREE_NEHE_H
#endif
//FreeType Headers
#include "TextRenderer.hpp"

//gl
#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <GL/GLU.h>

//imgui
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <iostream>
#include <functional>
#include "TextRenderer.hpp"
#include "Math.h"

namespace EventStream {

    struct MouseEvent {
        int x, y;
        int normal_x, normal_y;
        int button;
        int action;
        int mods;
        double scrollxoffset;
        double scrollyoffset;
    };
    struct KeyboardEvent {
        int key; int scancode; int action; int mods;
    };
    struct JoystickEvent {
        int jid;
        int evnt;
    };
    struct ErrorEvent {
        int error;
        const char* description;
    };

    struct Event {
        std::string EventType;
        MouseEvent mouse;
        KeyboardEvent keyboard;
        JoystickEvent joystick;
        ErrorEvent error;
    };

    class EventProcessor {
    public:
        virtual bool HandleEvent(Event e) {
            return true;
        };
    };
}

namespace Graphics {

    namespace Util {
        glm::vec3 HSVtoRGB(float H, float S, float V) {
            if (H > 360 || H < 0 || S>100 || S < 0 || V>100 || V < 0) {
                return glm::vec3(0);
            }
            float s = S / 100;
            float v = V / 100;
            float C = s * v;
            float X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
            float m = v - C;
            float r, g, b;
            if (H >= 0 && H < 60) {
                r = C, g = X, b = 0;
            }
            else if (H >= 60 && H < 120) {
                r = X, g = C, b = 0;
            }
            else if (H >= 120 && H < 180) {
                r = 0, g = C, b = X;
            }
            else if (H >= 180 && H < 240) {
                r = 0, g = X, b = C;
            }
            else if (H >= 240 && H < 300) {
                r = X, g = 0, b = C;
            }
            else {
                r = C, g = 0, b = X;
            }
            return glm::vec3((r + m) * 255, (g + m) * 255, (b + m) * 255);
        }

        glm::vec3 HSVtoRGB(glm::vec3 HSV) {
            float H = HSV.x;
            float S = HSV.y;
            float V = HSV.z;
            if (H > 360 || H < 0 || S>100 || S < 0 || V>100 || V < 0) {
                return glm::vec3(0);
            }
            float s = S / 100;
            float v = V / 100;
            float C = s * v;
            float X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
            float m = v - C;
            float r, g, b;
            if (H >= 0 && H < 60) {
                r = C, g = X, b = 0;
            }
            else if (H >= 60 && H < 120) {
                r = X, g = C, b = 0;
            }
            else if (H >= 120 && H < 180) {
                r = 0, g = C, b = X;
            }
            else if (H >= 180 && H < 240) {
                r = 0, g = X, b = C;
            }
            else if (H >= 240 && H < 300) {
                r = X, g = 0, b = C;
            }
            else {
                r = C, g = 0, b = X;
            }
            return glm::vec3((r + m), (g + m), (b + m));
        }
        float Dist(glm::vec2 one, glm::vec2 two) {
            glm::vec2 squared = (two - one) * (two - one);
            return sqrt(squared.x + squared.y);
        }
    }    

    GLFWwindow* InteractionWindow;
    
    //callbacks
    static void error_callback(int error, const char* description);

    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

    static void joystick_callback(int jid, int event);

    class Window {
    protected:
        std::string Title;
        GLFWwindow* window;

        std::function<void(GLFWwindow* wind, int sizeX, int sizeY)> Camera_function;
        std::function<void(GLFWwindow* wind, int sizeX, int sizeY)> Draw_function;
        std::function<void(GLFWwindow* wind, int sizeX, int sizeY)> GUI_function;

        std::vector<EventStream::Event> Events;
        std::vector<EventStream::EventProcessor*> EventProcessors;
    public:

        GLFWwindow* GetWindow() {
            return window;
        }

        Window(std::string title) {
            Title = title;
        }

        void AddEventProcessor(EventStream::EventProcessor* evp) {
            EventProcessors.push_back(evp);
        }

        void PushEvent(EventStream::Event e) {
            Events.push_back(e);
        }

        void ProcessEvents() {
            while (Events.size() > 0) {
                for (int i = 0; i < EventProcessors.size(); i++) {
                    //if should pass event to rest of event processors
                    if (!EventProcessors[i]->HandleEvent(Events[0])) {
                        Events.erase(Events.begin());
                        break;
                    }
                }
                if (Events.size() > 0) {
                    Events.erase(Events.begin());
                }
            }
        }

        bool Init() {
            if (!glfwInit())
                return false;

            glfwSetErrorCallback(error_callback);

            // GL 3.0 + GLSL 130
            const char* glsl_version = "#version 130";
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
            //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
            //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only

            // Create window with graphics context
            window = glfwCreateWindow(720, 720, Title.c_str(), NULL, NULL);
            if (window == NULL)
                return false;
            InteractionWindow = window;
            glfwMakeContextCurrent(window);
            glfwSwapInterval(1); // Enable vsync

            //imgui setup
            // Setup Dear ImGui context
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO();
            // Setup Platform/Renderer bindings
            ImGui_ImplOpenGL3_Init(glsl_version);
            ImGui_ImplGlfw_InitForOpenGL(window, true);

            // Setup Dear ImGui style
            ImGui::StyleColorsDark();

            glfwSetKeyCallback(window, key_callback);
            glfwSetCursorPosCallback(window, cursor_position_callback);
            glfwSetMouseButtonCallback(window, mouse_button_callback);
            glfwSetScrollCallback(window, scroll_callback);
            glfwSetJoystickCallback(joystick_callback);

            glfwMakeContextCurrent(window);
            glfwSwapInterval(1);

            // NOTE: OpenGL error checks have been omitted for brevity

            glEnable(GL_DEPTH_TEST); // Depth Testing
            glDepthFunc(GL_LEQUAL);
            glDisable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            return true;
        }

        int Loop() {
            while (!glfwWindowShouldClose(window))
            {
                GLint windowWidth, windowHeight;
                glfwGetWindowSize(window, &windowWidth, &windowHeight);
                glViewport(0, 0, windowWidth, windowHeight);

                ProcessEvents();
                // Draw stuff
                glClearColor(0.0, 00, 00, 1.0);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                glMatrixMode(GL_PROJECTION_MATRIX);
                glLoadIdentity();
                //transform world here
                //camera
                Camera_function(window, windowWidth, windowHeight);
                //end
                //end
                glMatrixMode(GL_MODELVIEW_MATRIX);
                //draw objects here
                Draw_function(window, windowWidth, windowHeight);
                //end
                //gui
                ImGui_ImplOpenGL3_NewFrame();
                ImGui_ImplGlfw_NewFrame();
                ImGui::NewFrame();
                GUI_function(window, windowWidth, windowHeight);
                ImGui::Render();
                ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
                //end
                glfwSwapBuffers(window);
                glfwPollEvents();
            }
            return 0;
        }

        int CleanUp() {
            //ImGui_ImplOpenGL3_Shutdown();
            //ImGui_ImplGlfw_Shutdown();
            //ImGui::DestroyContext();
            //glfwDestroyWindow(window);

            //glfwTerminate();
            exit(EXIT_SUCCESS);
        }
        ~Window() {
            CleanUp();
        }

        void Set_Camera_function(std::function<void(GLFWwindow* wind, int sizeX, int sizeY)> cf) {
            Camera_function = cf;
        };
        void Set_Draw_function(std::function<void(GLFWwindow* wind, int sizeX, int sizeY)>df) {
            Draw_function = df;
        };
        void Set_GUI_function(std::function<void(GLFWwindow* wind, int sizeX, int sizeY)>gf) {
            GUI_function = gf;
        };
    };

    Window* Callback_Window;

    void SetCallBackWindow(Window* Cb_Window) {
        Callback_Window = Cb_Window;
    }

    static void error_callback(int error, const char* description)
    {
        EventStream::Event e;
        e.error.error = error;
        e.error.description = description;
        e.EventType = "ErrorEvent";
        Callback_Window->PushEvent(e);
    }

    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
        ImGuiIO& io = ImGui::GetIO();
        if (!io.WantCaptureKeyboard) {
            //PROCESS KEYBOARD HERE
            EventStream::Event e;
            e.keyboard.key = key;
            e.keyboard.scancode = scancode;
            e.keyboard.action = action;
            e.keyboard.mods = mods;
            e.EventType = "KeyboardEvent";
            Callback_Window->PushEvent(e);
        }
    }

    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
    {
        ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);
        ImGuiIO& io = ImGui::GetIO();
        if (!io.WantCaptureMouse) {
            //PROCESS MOUSE HERE
            EventStream::Event e;
            e.mouse.x = xpos;
            e.mouse.y = ypos;
            int width, height = 0;
            glfwGetWindowSize(InteractionWindow, &width, &height);
            e.mouse.normal_x = (((float)xpos / (float)width) - .5) * 2;
            e.mouse.normal_y = ((1 - ((float)ypos / (float)height)) - .5) * 2;
            e.EventType = "MouseMoveEvent";
            Callback_Window->PushEvent(e);
        }
    }

    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
    {
        ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
        ImGuiIO& io = ImGui::GetIO();
        if (!io.WantCaptureMouse) {
            //PROCESS MOUSE HERE
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            EventStream::Event e;
            e.mouse.action = action;
            e.mouse.button = button;
            e.mouse.mods = mods;
            e.mouse.x = xpos;
            e.mouse.y = ypos;
            int width, height = 0;
            glfwGetWindowSize(InteractionWindow, &width, &height);
            e.mouse.normal_x = (((float)xpos / (float)width) - .5) * 2;
            e.mouse.normal_y = ((1 - ((float)ypos / (float)height)) - .5) * 2;
            e.EventType = "MouseButtonEvent";
            Callback_Window->PushEvent(e);
        }
    }

    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
    {
        ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
        ImGuiIO& io = ImGui::GetIO();
        if (!io.WantCaptureMouse) {
            //PROCESS MOUSE HERE
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            EventStream::Event e;
            e.mouse.scrollxoffset = xoffset;
            e.mouse.scrollyoffset = yoffset;
            e.mouse.x = xpos;
            e.mouse.y = ypos;
            int width, height = 0;
            glfwGetWindowSize(InteractionWindow, &width, &height);
            e.mouse.normal_x = (((float)xpos / (float)width) - .5) * 2;
            e.mouse.normal_y = ((1 - ((float)ypos / (float)height)) - .5) * 2;
            e.EventType = "MouseScrollEvent";
            Callback_Window->PushEvent(e);
        }
    }

    static void joystick_callback(int jid, int event)
    {
        EventStream::Event e;
        e.joystick.evnt = event;
        e.joystick.jid = jid;
        e.EventType = "JoystickEvent";
        Callback_Window->PushEvent(e);
    }

    namespace MeshTools {
        class Mesh {
            std::vector<Math::Vec3<float>> Verts;
            std::vector<Math::Vec2<int>> Edges;
            std::vector<std::vector<int>> Faces;
            Math::Vec3<float> position,scale,rotation;
        };


    }

}