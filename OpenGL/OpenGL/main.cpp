#include <stdio.h>
#include <string.h>
#include <cmath>

// GLEW 
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;

//VAO VBOS
GLuint VAO, VBO, shader, uniformModel;

bool bDirection = true;
float triangleOffset = 0.0f;
float triangleMaxOffset = 0.7f;
float triangleIncrement = 0.005f;

// Vertex Shader
static const char* pVShader = "                                     \n\
#version 330                                                        \n\
                                                                    \n\
layout (location = 0) in vec3 pos;                                  \n\
                                                                    \n\
uniform mat4 model;                                                \n\
                                                                    \n\
void main()                                                         \n\
{                                                                   \n\
    gl_Position = model * vec4(0.4 * pos.x, 0.4 * pos.y, pos.z, 1.0);       \n\
}";

// Fragment Shader
static const char* pFShader = "                                     \n\
#version 330                                                        \n\
                                                                    \n\
out vec4 colour;                                 \n\
                                                                    \n\
void main()                                                         \n\
{                                                                   \n\
    colour = vec4(1.0, 0.0, 0.0, 1.0);       \n\
}";

void CreateTriangle()
{
    GLfloat vertices[] =
    {
        -1.0f, -1.0f, 0.0f, // Bottom left corner
        1.0f, -1.0f, 0.0f,  // Bottom right corner
        0.0f, 1.0f, 0.0f    // Top corner
    };

    glGenVertexArrays(1, &VAO); // Generate VAO
    glBindVertexArray(VAO);     // Bind VAO

    glGenBuffers(1, &VBO); // Generate VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // Bind VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Copy vertices to VBO

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // Specify layout of VBO
    glEnableVertexAttribArray(0); // Enable vertex attribute array 0

    glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind VBO
    glBindVertexArray(0); // Unbind VAO
}

void AddShader(GLuint t_program, const char* t_shaderCode, GLenum t_shaderType)
{
    GLuint shader = glCreateShader(t_shaderType); // Create shader
    
    const GLchar* code[1];
    code[0] = t_shaderCode;

    GLint codeLength[1];
    codeLength[0] = strlen(t_shaderCode);

    glShaderSource(shader, 1, code, codeLength); // Attach shader code
    glCompileShader(shader); // Compile shader

    GLint result = 0;
    GLchar eLog[1024] = { 0 };
    
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("Error compiling the %d shader: '%s'\n", t_shaderType, eLog);
        return;
    }

    glAttachShader(t_program, shader); // Attach shader to program
}

void AssembleShaders()
{
    shader = glCreateProgram(); // Create shader program
    if (!shader)
    {
        printf("Error creating shader program!\n");
        return;
    }

    AddShader(shader, pVShader, GL_VERTEX_SHADER); // Add vertex shader
    AddShader(shader, pFShader, GL_FRAGMENT_SHADER); // Add fragment shader

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glLinkProgram(shader); // Link shader program
    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("Error linking program: '%s'\n", eLog);
        return;
    }

    glValidateProgram(shader); // Validate shader program
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("Error validating program: '%s'\n", eLog);
        return;
    }

    uniformModel = glGetUniformLocation(shader, "model");
}

int main()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        printf("GLFW initialization failed!");
        glfwTerminate();
        return 1;
    }
    
    //setup GLFW window properties
    //OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //Core profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //Allow forward compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *pMainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);
    // Check if the window was created
    if (!pMainWindow)
    {
        printf("GLFW window creation failed!");
        glfwTerminate();
        return 1;
    }

    // Get buffer size information
    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(pMainWindow, &bufferWidth, &bufferHeight);

    // Set context for GLEW to use
    glfwMakeContextCurrent(pMainWindow);

    // allow experimental features
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        printf("GLEW initialization failed!");
        glfwDestroyWindow(pMainWindow); // Destroy the window
        glfwTerminate();
        return 1;
    }

    //setup viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);

    CreateTriangle();
    AssembleShaders();
    

    // Loop until window closed
    while (!glfwWindowShouldClose(pMainWindow))
    {
        // Get + Handle user input events
        glfwPollEvents();

        if (bDirection)
        {
            triangleOffset += triangleIncrement;
        }
        else
        {
            triangleOffset -= triangleIncrement;
        }

        if (abs(triangleOffset) >= triangleMaxOffset)
        {
            bDirection = !bDirection;
        }

        // Clear window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Red
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader); // Use shader program

        glm::mat4 model = glm::mat4(1.0f); // Create identity matrix its all values are 0 except the diagonals
        model = glm::translate(model, glm::vec3(triangleOffset, triangleOffset, 0.0f)); // Translate matrix
        
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); // Set uniform variable

        glBindVertexArray(VAO); // Bind VAO
        glDrawArrays(GL_TRIANGLES, 0, 3); // Draw triangle
        
        glBindVertexArray(0); // Unbind VAO
        glUseProgram(0); // Unuse shader program
        
        glfwSwapBuffers(pMainWindow);
    }
    
    return 0;
}
