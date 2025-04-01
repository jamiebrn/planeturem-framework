#pragma once

#include <gl.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

namespace pl
{

class Shader
{
public:
    Shader() = default;
    ~Shader();
    Shader(Shader& shader) = delete;

    bool load(const std::string& vertexPath, const std::string& fragPath);

    void use() const;
    GLuint getProgram();

private:
    GLuint shaderProgram = 0;

};

}