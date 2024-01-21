#pragma once
#include <random>
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//plog headers
#include <plog/Log.h>
#include <plog/Init.h>
#include <plog/Appenders/ColorConsoleAppender.h>
#include <plog/Appenders/RollingFileAppender.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Formatters/FuncMessageFormatter.h>

namespace plog {
    template<class Formatter> // Typically a formatter is passed as a template parameter.
    class VisibleAppender : public IAppender // All appenders MUST inherit IAppender interface.
    {
    public:
        void write(const Record& record) override // This is a method from IAppender that MUST be implemented.
        {
            util::nstring str = Formatter::format(record); // Use the formatter to get a string from a record.
			if (str.empty())
				return;
            if (m_messageList.size() > 3000) {
                m_messageList.erase(m_messageList.begin());
            }
            m_messageList.push_back(std::string(str.begin(), str.end())); // Store a log message in a list.
        }

        std::vector<std::string>& getMessageList()
        {
            return m_messageList;
        }
    private:
        std::vector<std::string> m_messageList;
    };
}

namespace Util {
    static float clamp(float val, float min, float max) {
        if (val < min) {
            return min;
        }
        if (val > max) {
            return max;
        }
        return val;
    }

    static glm::vec3 PointOnSphere(float radius, float StickX, float StickY) {
        glm::vec3 point = { 0,1,0 };
        //glm::rotate
        glm::vec4 returnpoint = glm::rotate(glm::mat4(1), StickY, glm::vec3(1, 0, 0)) * glm::vec4(point, 1);
        if (StickX > 0) {
            returnpoint = glm::rotate(glm::mat4(1), StickX, glm::vec3(0, 1, 0)) * returnpoint;
        }
        else {
            returnpoint = glm::rotate(glm::mat4(1), StickX, glm::vec3(0, 1, 0)) * returnpoint;
        }
        return glm::vec3(returnpoint) * radius;
    }

    template<typename T>
    T random(T range_from, T range_to) {
        std::random_device                  rand_dev;
        std::mt19937                        generator(rand_dev());
        std::uniform_real_distribution<T>    distr(range_from, range_to);
        return distr(generator);
    }
    template<>
    int random(int range_from, int range_to);

    class Exception {
        std::string message;
        std::string error;
        int id;
    public:
        Exception(std::string message, std::string error, int id = 0) : message(message), error(error), id(id) {};
        std::string GetMessage() { return message; };
        std::string GetError() { return error; };
        int GetID() { return id; };
        void Print() { std::cout << "Error: " << error << " Message: " << message << " ID: " << id << std::endl; };
    };

    static glm::vec3 HSVtoRGB(float H, float S, float V) {
        if (H > 360 || H < 0 || S>100 || S < 0 || V>100 || V < 0) {
            return glm::vec3();
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
        float R = (r + m);
        float G = (g + m);
        float B = (b + m);
        return glm::vec3(R, G, B);
    }

    static glm::vec3 HSVtoRGB(glm::vec3 HSV) {
        return HSVtoRGB(HSV.x, HSV.y, HSV.z);
    }
    static glm::vec4 HSVtoRGBA(float H, float S, float V, float A) {
        glm::vec3 rgb = HSVtoRGB(H, S, V);
        return glm::vec4(rgb.x, rgb.y, rgb.z, A);
    }
    //lerp template
	template<typename T>
	T lerp(T a, T b, float t) {
		return a + (b - a) * t;
	}
    //map range
	template<typename T>
	T map(T value, T fromLow, T fromHigh, T toLow, T toHigh) {
		return (value - fromLow) / (fromHigh - fromLow) * (toHigh - toLow) + toLow;
	}

    namespace Logs {
        enum Logs_T {
            Debug,
            Error,
            Info
        };


        static plog::VisibleAppender<plog::TxtFormatter> InfoLog, ErrorLog, DebugLog;

		static void InitLogs() {
            plog::init<Logs_T::Info>(plog::none, new plog::RollingFileAppender<plog::TxtFormatter>("Info.log", 1000, 5)).addAppender(&InfoLog);
			plog::init<Logs_T::Error>(plog::error, new plog::RollingFileAppender<plog::TxtFormatter>("Error.log", 1000, 5)).addAppender(&ErrorLog);
			plog::init<Logs_T::Debug>(plog::debug, new plog::RollingFileAppender<plog::TxtFormatter>("Debug.log", 1000, 5)).addAppender(&DebugLog);
		}
    }
}