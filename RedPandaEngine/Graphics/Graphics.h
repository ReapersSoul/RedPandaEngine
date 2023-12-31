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
// gl
#include <GL/glew.h>
#include <GL/glu.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

// imgui
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

// plog headers
#include <plog/Log.h>
#include <plog/Init.h>
#include <plog/Appenders/ColorConsoleAppender.h>
#include <plog/Appenders/RollingFileAppender.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Formatters/FuncMessageFormatter.h>

#include <Utility/util.h>

#include <iostream>
#include <functional>

namespace EventStream
{
    struct Event
    {
        std::string EventType;
    };
    struct MouseEvent : public Event
    {
        int x, y;
        int normal_x, normal_y;
        int button;
        int action;
        int mods;
        double scrollxoffset;
        double scrollyoffset;
    };
    struct KeyboardEvent : public Event
    {
        int key;
        int scancode;
        int action;
        int mods;
    };
    struct JoystickEvent : public Event
    {
        int jid;
        int evnt;
    };
    struct ErrorEvent : public Event
    {
        int error;
        const char *description;
    };

    class EventProcessor
    {
    public:
        virtual bool HandleEvent(Event *e)
        {
            return true;
        };
    };
}

namespace Graphics
{
    static void glColor(glm::vec3 color)
    {
        glColor3f(color.x, color.y, color.z);
    }
    static void glColor(glm::vec4 color)
    {
        glColor4f(color.x, color.y, color.z, color.w);
    }

    static GLFWwindow *InteractionWindow;

    // callbacks
    static void error_callback(int error, const char *description);

    static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

    static void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);

    static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);

    static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

    static void joystick_callback(int jid, int event);

    class Window
    {
    protected:
        std::string Title;
        GLFWwindow *window;

        std::function<void(GLFWwindow *wind, int sizeX, int sizeY)> Camera_function;
        std::function<void(GLFWwindow *wind, int sizeX, int sizeY)> Draw_function;
        std::function<void(GLFWwindow *wind, int sizeX, int sizeY)> GUI_function;
        std::function<void(GLFWwindow *wind, int sizeX, int sizeY)> Update_function;

        std::vector<EventStream::Event> Events;
        std::vector<EventStream::EventProcessor *> EventProcessors;

    public:
        GLFWwindow *GetWindow()
        {
            return window;
        }

        Window(std::string title)
        {
            Title = title;
        }

        void AddEventProcessor(EventStream::EventProcessor *evp)
        {
            EventProcessors.push_back(evp);
        }

        void PushEvent(EventStream::Event e)
        {
            Events.push_back(e);
        }

        void ProcessEvents()
        {
            while (Events.size() > 0)
            {
                for (int i = 0; i < EventProcessors.size(); i++)
                {
                    // if should pass event to rest of event processors
                    if (!EventProcessors[i]->HandleEvent(&Events[0]))
                    {
                        Events.erase(Events.begin());
                        break;
                    }
                }
                if (Events.size() > 0)
                {
                    Events.erase(Events.begin());
                }
            }
        }

        bool Init()
        {
            if (!glfwInit())
                return false;

            glfwSetErrorCallback(error_callback);

            // GL 3.0 + GLSL 130
            const char *glsl_version = "#version 130";
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
            // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
            // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only

            // Create window with graphics context
            window = glfwCreateWindow(720, 720, Title.c_str(), NULL, NULL);
            if (window == NULL)
                return false;
            InteractionWindow = window;
            glfwMakeContextCurrent(window);
            glfwSwapInterval(1); // Enable vsync

            GLenum glewError = glewInit();
            if (glewError != GLEW_OK)
            {
                // Handle error
                PLOGD_(Util::Logs::Error) << "GLEW Error: " << glewGetErrorString(glewError) << "\n";
                return false;
            }

            // imgui setup
            //  Setup Dear ImGui context
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO &io = ImGui::GetIO();
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

            glEnable(GL_DEPTH_TEST); // Depth Testing
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glDepthFunc(GL_LEQUAL);
            glDisable(GL_CULL_FACE);
            glCullFace(GL_BACK);

            return true;
        }

        int Loop()
        {
            while (!glfwWindowShouldClose(window))
            {
                GLint windowWidth, windowHeight;
                glfwGetWindowSize(window, &windowWidth, &windowHeight);
                glViewport(0, 0, windowWidth, windowHeight);

                ProcessEvents();
                // Draw stuff
                glClearColor(0.0, 0.0, 0.0, 1.0);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                // update here
                Update_function(window, windowWidth, windowHeight);
                glMatrixMode(GL_PROJECTION_MATRIX);
                glLoadIdentity();
                // transform world here
                // camera
                Camera_function(window, windowWidth, windowHeight);
                // end
                // end
                glMatrixMode(GL_MODELVIEW_MATRIX);
                // draw objects here
                Draw_function(window, windowWidth, windowHeight);
                // end
                // gui
                ImGui_ImplOpenGL3_NewFrame();
                ImGui_ImplGlfw_NewFrame();
                ImGui::NewFrame();
                GUI_function(window, windowWidth, windowHeight);
                ImGui::Render();
                ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
                // end
                glfwSwapBuffers(window);
                glfwPollEvents();
            }
            return 0;
        }

        int CleanUp()
        {
            glfwDestroyWindow(window);

            glfwTerminate();
            exit(EXIT_SUCCESS);
        }
        ~Window()
        {
            CleanUp();
        }

        void Set_Camera_function(std::function<void(GLFWwindow *wind, int sizeX, int sizeY)> cf)
        {
            Camera_function = cf;
        };
        void Set_Draw_function(std::function<void(GLFWwindow *wind, int sizeX, int sizeY)> df)
        {
            Draw_function = df;
        };
        void Set_GUI_function(std::function<void(GLFWwindow *wind, int sizeX, int sizeY)> gf)
        {
            GUI_function = gf;
        };
        void Set_Update_function(std::function<void(GLFWwindow *wind, int sizeX, int sizeY)> gf)
        {
            Update_function = gf;
        };
    };

    static Window *Callback_Window;

    static void SetCallBackWindow(Window *Cb_Window)
    {
        Callback_Window = Cb_Window;
    }

    static void error_callback(int error, const char *description)
    {
        EventStream::ErrorEvent e;
        e.error = error;
        e.description = description;
        e.EventType = "ErrorEvent";
        Callback_Window->PushEvent(e);
    }

    static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
        ImGuiIO &io = ImGui::GetIO();
        if (!io.WantCaptureKeyboard)
        {
            // PROCESS KEYBOARD HERE
            EventStream::KeyboardEvent e;
            e.key = key;
            e.scancode = scancode;
            e.action = action;
            e.mods = mods;
            e.EventType = "KeyboardEvent";
            Callback_Window->PushEvent(e);
        }
    }

    static void cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
    {
        ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);
        ImGuiIO &io = ImGui::GetIO();
        if (!io.WantCaptureMouse)
        {
            // PROCESS MOUSE HERE
            EventStream::MouseEvent e;
            e.x = xpos;
            e.y = ypos;
            int width, height = 0;
            glfwGetWindowSize(InteractionWindow, &width, &height);
            e.normal_x = (((float)xpos / (float)width) - .5) * 2;
            e.normal_y = ((1 - ((float)ypos / (float)height)) - .5) * 2;
            e.EventType = "MouseMoveEvent";
            Callback_Window->PushEvent(e);
        }
    }

    static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
    {
        ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
        ImGuiIO &io = ImGui::GetIO();
        if (!io.WantCaptureMouse)
        {
            // PROCESS MOUSE HERE
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            EventStream::MouseEvent e;
            e.action = action;
            e.button = button;
            e.mods = mods;
            e.x = xpos;
            e.y = ypos;
            int width, height = 0;
            glfwGetWindowSize(InteractionWindow, &width, &height);
            e.normal_x = (((float)xpos / (float)width) - .5) * 2;
            e.normal_y = ((1 - ((float)ypos / (float)height)) - .5) * 2;
            e.EventType = "MouseButtonEvent";
            Callback_Window->PushEvent(e);
        }
    }

    static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
    {
        ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
        ImGuiIO &io = ImGui::GetIO();
        if (!io.WantCaptureMouse)
        {
            // PROCESS MOUSE HERE
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            EventStream::MouseEvent e;
            e.scrollxoffset = xoffset;
            e.scrollyoffset = yoffset;
            e.x = xpos;
            e.y = ypos;
            int width, height = 0;
            glfwGetWindowSize(InteractionWindow, &width, &height);
            e.normal_x = (((float)xpos / (float)width) - .5) * 2;
            e.normal_y = ((1 - ((float)ypos / (float)height)) - .5) * 2;
            e.EventType = "MouseScrollEvent";
            Callback_Window->PushEvent(e);
        }
    }

    static void joystick_callback(int jid, int event)
    {
        EventStream::JoystickEvent e;
        e.evnt = event;
        e.jid = jid;
        e.EventType = "JoystickEvent";
        Callback_Window->PushEvent(e);
    }

    namespace TextureTools
    {
        class Texture
        {
            GLuint ID;
            int width, height, depth;
        };
    }

    namespace MeshTools
    {
        class Mesh
        {
        protected:
            std::vector<glm::vec3> Verts;
            std::vector<glm::vec2> Edges;
            std::vector<glm::vec3> TexCoOrd;
            std::vector<std::vector<int>> Faces;
            glm::vec3 position, scale, rotation;
            std::vector<TextureTools::Texture *> Textures;

            std::vector<glm::vec3> ApplyTransform();

        public:
            void Draw();
        };

        namespace Shapes
        {
            class Quad : public Mesh
            {
            public:
                Quad();
                ~Quad();
            };
            class Triangle : public Mesh
            {
            public:
                Triangle();
                ~Triangle();
            };

            class Cube : public Mesh
            {
            public:
                Cube();
                ~Cube();
                bool Within(glm::vec3 point);
                std::vector<glm::vec3> Within(Cube other);
            };
            class Pyramid : public Mesh
            {
            public:
                Pyramid();
                ~Pyramid();
            };
            class Sphere : public Mesh
            {
            public:
                Sphere();
                ~Sphere();
            };
        }
    }

    namespace ViewPortTools
    {
        class ViewPort
        {
        };

        class Camera
        {
        };
    }

}