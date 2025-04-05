#pragma once

#include <gl.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

#include "Vector.hpp"
#include "Color.hpp"
#include "Texture.hpp"

namespace pl
{

class Shader
{
public:
    Shader() = default;
    ~Shader();
    Shader(Shader& shader) = delete;

    bool load(const std::string& vertexPath, const std::string& fragPath);

    void bind() const;
    GLuint getProgram();

    void setUniform1f(const std::string& name, float v0);

    void setUniform2f(const std::string& name, float v0, float v1);
    
    void setUniform3f(const std::string& name, float v0, float v1, float v2);
    
    void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    
    void setUniform1i(const std::string& name, int v0);
    
    void setUniform2i(const std::string& name, int v0, int v1);
    
    void setUniform3i(const std::string& name, int v0, int v1, int v2);
    
    void setUniform4i(const std::string& name, int v0, int v1, int v2, int v3);
    
    void setUniform1ui(const std::string& name, uint32_t v0);
    
    void setUniform2ui(const std::string& name, uint32_t v0, uint32_t v1);
    
    void setUniform3ui(const std::string& name, uint32_t v0, uint32_t v1, uint32_t v2);
    
    void setUniform4ui(const std::string& name, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3);
    
    void setUniform1fv(const std::string& name, const std::vector<float>& values);
    
    void setUniform2fv(const std::string& name, const std::vector<float>& values);
    
    void setUniform3fv(const std::string& name, const std::vector<float>& values);
    
    void setUniform4fv(const std::string& name, const std::vector<float>& values);

    void setUniform1iv(const std::string& name, const std::vector<int>& values);
    
    void setUniform2iv(const std::string& name, const std::vector<int>& values);
    
    void setUniform3iv(const std::string& name, const std::vector<int>& values);
    
    void setUniform4iv(const std::string& name, const std::vector<int>& values);

    void setUniform1uiv(const std::string& name, const std::vector<uint32_t>& values);
    
    void setUniform2uiv(const std::string& name, const std::vector<uint32_t>& values);
    
    void setUniform3uiv(const std::string& name, const std::vector<uint32_t>& values);
    
    void setUniform4uiv(const std::string& name, const std::vector<uint32_t>& values);

    void setUniformColor(const std::string& name, const Color& color);

    void setUniformTexture(const std::string& name, const Texture& texture);

private:
    int getUniformLocation(const std::string& uniformName);

    GLuint shaderProgram = 0;

    std::unordered_map<std::string, int> uniformLocations;
    std::unordered_map<int, const Texture*> textureBindings;

};

}