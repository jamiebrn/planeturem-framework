#include "Shader.hpp"

pl::Shader::~Shader()
{
    if (shaderProgram == 0)
    {
        return;
    }

    glDeleteProgram(shaderProgram);
    shaderProgram = 0;
}

bool pl::Shader::load(const std::string& vertexPath, const std::string& fragPath)
{
    std::ifstream vertexStream(vertexPath);

    if (!vertexStream)
    {
        return false;
    }

    std::stringstream vertexBuffer;
    vertexBuffer << vertexStream.rdbuf();
    std::string vertexString;
    vertexString = vertexBuffer.str();

    std::ifstream fragStream(fragPath);

    if (!fragStream)
    {
        return false;
    }

    std::stringstream fragBuffer;
    fragBuffer << fragStream.rdbuf();
    std::string fragString;
    fragString = fragBuffer.str();

    vertexStream.close();
    fragStream.close();

    const char* vertexCStr = vertexString.c_str();
    const char* fragCStr = fragString.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexCStr, NULL);
    glCompileShader(vertexShader);

    GLint vertexShaderCompiled;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexShaderCompiled);
    if (!vertexShaderCompiled)
    {
        GLchar infoLog[256];
        glGetShaderInfoLog(vertexShader, sizeof(infoLog), NULL, infoLog);
        std::cout << "ERROR: Failed to compile vertex shader\n" << infoLog << "\n";
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragCStr, NULL);
    glCompileShader(fragmentShader);

    GLint fragmentShaderCompiled;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentShaderCompiled);
    if (!fragmentShaderCompiled)
    {
        GLchar infoLog[256];
        glGetShaderInfoLog(fragmentShader, sizeof(infoLog), NULL, infoLog);
        std::cout << "ERROR: Failed to compile fragment shader\n" << infoLog << "\n";
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLint linkSuccess;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkSuccess);
    if (!linkSuccess)
    {
        GLchar infoLog[256];
        glGetProgramInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
        std::cout << "ERROR: Failed to link shader\n" << infoLog << "\n";
    }

    return true;
}

void pl::Shader::bind() const
{
    if (shaderProgram == 0)
    {
        return;
    }

    glUseProgram(shaderProgram);

    int textureIdx = 1;
    for (auto iter = textureBindings.begin(); iter != textureBindings.end(); iter++)
    {
        glUniform1i(iter->first, textureIdx);
        iter->second->use(textureIdx);
        textureIdx++;
    }

    glActiveTexture(GL_TEXTURE0);
}

GLuint pl::Shader::getProgram()
{
    return shaderProgram;
}

int pl::Shader::getUniformLocation(const std::string& uniformName)
{
    if (uniformLocations.contains(uniformName))
    {
        return uniformLocations.at(uniformName);
    }

    int location = glGetUniformLocation(shaderProgram, uniformName.c_str());

    if (location < 0)
    {
        printf(("ERROR: Could not find uniform \"" + uniformName + "\"\n").c_str());
    }
    else
    {
        uniformLocations[uniformName] = location;
    }

    return location;
}

void pl::Shader::setUniform1f(const std::string& name, float v0)
{
    int location = getUniformLocation(name);
    if (location >= 0)
    {
        glUseProgram(shaderProgram);
        glUniform1f(location, v0);
    }
    glUseProgram(0);
}

void pl::Shader::setUniform2f(const std::string& name, float v0, float v1)
{
    int location = getUniformLocation(name);
    if (location >= 0)
    {
        glUseProgram(shaderProgram);
        glUniform2f(location, v0, v1);
    }
    glUseProgram(0);
}

void pl::Shader::setUniform3f(const std::string& name, float v0, float v1, float v2)
{
    int location = getUniformLocation(name);
    if (location >= 0)
    {
        glUseProgram(shaderProgram);
        glUniform3f(location, v0, v1, v2);
    }
    glUseProgram(0);
}

void pl::Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    int location = getUniformLocation(name);
    if (location >= 0)
    {
        glUseProgram(shaderProgram);
        glUniform4f(location, v0, v1, v2, v3);
    }
    glUseProgram(0);
}

void pl::Shader::setUniform1i(const std::string& name, int v0)
{
    int location = getUniformLocation(name);
    if (location >= 0)
    {
        glUseProgram(shaderProgram);
        glUniform1i(location, v0);
    }
    glUseProgram(0);
}

void pl::Shader::setUniform2i(const std::string& name, int v0, int v1)
{
    int location = getUniformLocation(name);
    if (location >= 0)
    {
        glUseProgram(shaderProgram);
        glUniform2i(location, v0, v1);
    }
    glUseProgram(0);
}

void pl::Shader::setUniform3i(const std::string& name, int v0, int v1, int v2)
{
    int location = getUniformLocation(name);
    if (location >= 0)
    {
        glUseProgram(shaderProgram);
        glUniform3i(location, v0, v1, v2);
    }
    glUseProgram(0);
}

void pl::Shader::setUniform4i(const std::string& name, int v0, int v1, int v2, int v3)
{
    int location = getUniformLocation(name);
    if (location >= 0)
    {
        glUseProgram(shaderProgram);
        glUniform4i(location, v0, v1, v2, v3);
    }
    glUseProgram(0);
}

void pl::Shader::setUniform1ui(const std::string& name, uint32_t v0)
{
    int location = getUniformLocation(name);
    if (location >= 0)
    {
        glUseProgram(shaderProgram);
        glUniform1ui(location, v0);
    }
    glUseProgram(0);
}

void pl::Shader::setUniform2ui(const std::string& name, uint32_t v0, uint32_t v1)
{
    int location = getUniformLocation(name);
    if (location >= 0)
    {
        glUseProgram(shaderProgram);
        glUniform2ui(location, v0, v1);
    }
    glUseProgram(0);
}

void pl::Shader::setUniform3ui(const std::string& name, uint32_t v0, uint32_t v1, uint32_t v2)
{
    int location = getUniformLocation(name);
    if (location >= 0)
    {
        glUseProgram(shaderProgram);
        glUniform3ui(location, v0, v1, v2);
    }
    glUseProgram(0);
}

void pl::Shader::setUniform4ui(const std::string& name, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3)
{
    int location = getUniformLocation(name);
    if (location >= 0)
    {
        glUseProgram(shaderProgram);
        glUniform4ui(location, v0, v1, v2, v3);
    }
    glUseProgram(0);
}

void pl::Shader::setUniform1fv(const std::string& name, const std::vector<float>& values)
{
    int location = getUniformLocation(name);
    if (location >= 0)
    {
        glUseProgram(shaderProgram);
        glUniform1fv(location, values.size(), values.data());
    }
    glUseProgram(0);
}

void pl::Shader::setUniform2fv(const std::string& name, const std::vector<float>& values)
{
    int location = getUniformLocation(name);
    if (location >= 0)
    {
        glUseProgram(shaderProgram);
        glUniform2fv(location, values.size(), values.data());
    }
    glUseProgram(0);
}

void pl::Shader::setUniform3fv(const std::string& name, const std::vector<float>& values)
{
    int location = getUniformLocation(name);
    if (location >= 0)
    {
        glUseProgram(shaderProgram);
        glUniform3fv(location, values.size(), values.data());
    }
    glUseProgram(0);
}

void pl::Shader::setUniform4fv(const std::string& name, const std::vector<float>& values)
{
    int location = getUniformLocation(name);
    if (location >= 0)
    {
        glUseProgram(shaderProgram);
        glUniform4fv(location, values.size(), values.data());
    }
    glUseProgram(0);
}

void pl::Shader::setUniform1iv(const std::string& name, const std::vector<int>& values)
{
    int location = getUniformLocation(name);
    if (location >= 0)
    {
        glUseProgram(shaderProgram);
        glUniform1iv(location, values.size(), values.data());
    }
    glUseProgram(0);
}

void pl::Shader::setUniform2iv(const std::string& name, const std::vector<int>& values)
{
    int location = getUniformLocation(name);
    if (location >= 0)
    {
        glUseProgram(shaderProgram);
        glUniform2iv(location, values.size(), values.data());
    }
    glUseProgram(0);
}

void pl::Shader::setUniform3iv(const std::string& name, const std::vector<int>& values)
{
    int location = getUniformLocation(name);
    if (location >= 0)
    {
        glUseProgram(shaderProgram);
        glUniform3iv(location, values.size(), values.data());
    }
    glUseProgram(0);
}

void pl::Shader::setUniform4iv(const std::string& name, const std::vector<int>& values)
{
    int location = getUniformLocation(name);
    if (location >= 0)
    {
        glUseProgram(shaderProgram);
        glUniform4iv(location, values.size(), values.data());
    }
    glUseProgram(0);
}

void pl::Shader::setUniform1uiv(const std::string& name, const std::vector<uint32_t>& values)
{
    int location = getUniformLocation(name);
    if (location >= 0)
    {
        glUseProgram(shaderProgram);
        glUniform1uiv(location, values.size(), values.data());
    }
    glUseProgram(0);
}

void pl::Shader::setUniform2uiv(const std::string& name, const std::vector<uint32_t>& values)
{
    int location = getUniformLocation(name);
    if (location >= 0)
    {
        glUseProgram(shaderProgram);
        glUniform2uiv(location, values.size(), values.data());
    }
    glUseProgram(0);
}

void pl::Shader::setUniform3uiv(const std::string& name, const std::vector<uint32_t>& values)
{
    int location = getUniformLocation(name);
    if (location >= 0)
    {
        glUseProgram(shaderProgram);
        glUniform3uiv(location, values.size(), values.data());
    }
    glUseProgram(0);
}

void pl::Shader::setUniform4uiv(const std::string& name, const std::vector<uint32_t>& values)
{
    int location = getUniformLocation(name);
    if (location >= 0)
    {
        glUseProgram(shaderProgram);
        glUniform4uiv(location, values.size(), values.data());
    }
    glUseProgram(0);
}

void pl::Shader::setUniformColor(const std::string& name, const Color& color)
{
    int location = getUniformLocation(name);
    if (location >= 0)
    {
        glUseProgram(shaderProgram);
        Color colorNormalised = color.normalise();
        glUniform4f(location, colorNormalised.r, colorNormalised.g, colorNormalised.b, colorNormalised.a);
    }
    glUseProgram(0);
}

void pl::Shader::setUniformTexture(const std::string& name, const Texture& texture)
{
    int location = getUniformLocation(name);
    if (location >= 0)
    {
        textureBindings[location] = &texture;
    }
}