#pragma once
#include <random>
#include <iostream>
#include <string>
#include <glm/glm.hpp>

namespace Util {
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
	
    static float Dist(glm::vec2 one, glm::vec2 two) {
        glm::vec2 squared = (two - one) * (two - one);
        return sqrt(squared.x + squared.y);
    }
}