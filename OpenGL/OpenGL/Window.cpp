#include "Window.h"

Window::Window()
{
    m_width = 800;
    m_height = 600;
}

Window::Window(GLint a_windowWidth, GLint a_windowHeight)
{
    m_width = a_windowWidth;
    m_height = a_windowHeight;
}

int Window::Initialise()
{
    if (!glfwInit())
    {
        printf("Error Initialising GLFW");
        glfwTerminate();
        return 1;
    }

    // Setup GLFW Windows Properties
    // OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Core Profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Allow forward compatiblity
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create the window
    m_pMainWindow = glfwCreateWindow(m_width, m_height, "Test Window", NULL, NULL);
    if (!m_pMainWindow)
    {
        printf("Error creating GLFW window!");
        glfwTerminate();
        return 1;
    }

    // Get buffer size information
    glfwGetFramebufferSize(m_pMainWindow, &m_bufferWidth, &m_bufferHeight);

    // Set the current context
    glfwMakeContextCurrent(m_pMainWindow);

    // Allow modern extension access
    glewExperimental = GL_TRUE;

    GLenum error = glewInit();
    if (error != GLEW_OK)
    {
        printf("Error: %s", glewGetErrorString(error));
        glfwDestroyWindow(m_pMainWindow);
        glfwTerminate();
        return 1;
    }

    glEnable(GL_DEPTH_TEST);

    // Create Viewport
    glViewport(0, 0, m_bufferWidth, m_bufferHeight);
}


Window::~Window()
{
    glfwDestroyWindow(m_pMainWindow);
    glfwTerminate();
}
