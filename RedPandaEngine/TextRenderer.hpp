#pragma once
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>
#include FT_FREETYPE_H
#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <GL/GLU.h>

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <memory>
#include <stdexcept>
namespace freetype {
    struct font_data {
        float h;  
        std::vector<GLuint> textures;
        GLuint list_base; 

        void init(const char* fname, unsigned int h);

        void clean();
    };
    void print(const font_data& ft_font, float x, float y, std::string const& text);

    void printf(const font_data& ft_font, float x, float y, const std::string format, ...);
}