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

void pl::Shader::use() const
{
    if (shaderProgram == 0)
    {
        return;
    }

    glUseProgram(shaderProgram);
}

GLuint pl::Shader::getProgram()
{
    return shaderProgram;
}