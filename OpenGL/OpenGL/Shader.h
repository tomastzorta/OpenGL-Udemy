#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL\glew.h>

class Shader
{
public:
    Shader();

    void CreateFromString(const char* a_vertexCode, const char* a_fragmentCode);
    void CreateFromFiles(const char* a_vertexLocation, const char* a_fragmentLocation);

    std::string ReadFile(const char* a_fileLocation);

    GLuint GetProjectionLocation();
    GLuint GetModelLocation();

    void UseShader();
    void ClearShader();

    ~Shader();

private:
    GLuint shaderID, uniformProjection, uniformModel;

    void M_CompileShader(const char* a_vertexCode, const char* a_fragmentCode);
    void M_AddShader(GLuint a_program, const char* a_shaderCode, GLenum a_shaderType);
};

