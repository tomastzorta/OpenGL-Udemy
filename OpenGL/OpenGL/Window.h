#pragma once

#include "stdio.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
public:
    Window();

    Window(GLint a_windowWidth, GLint a_windowHeight);

    int Initialise();

    GLint GetBufferWidth() { return m_bufferWidth; }
    GLint GetBufferHeight() { return m_bufferHeight; }

    bool GetShouldClose() { return glfwWindowShouldClose(m_pMainWindow); }

    void SwapBuffers() { glfwSwapBuffers(m_pMainWindow); }

    ~Window();

private:
    GLFWwindow* m_pMainWindow;

    GLint m_width, m_height;
    GLint m_bufferWidth, m_bufferHeight;
};