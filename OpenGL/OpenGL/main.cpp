#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"

const float toRadians = 3.14159265f / 180.0f;

Window g_mainWindow;
std::vector<Mesh*> g_meshList;
std::vector<Shader> g_shaderList;

// Vertex Shader
static const char* g_vShader = "shader.vert";

// Fragment Shader
static const char* g_fShader = "shader.frag";

void CreateObjects() 
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	g_meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 12, 12);
	g_meshList.push_back(obj2);
}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(g_vShader, g_fShader);
	g_shaderList.push_back(*shader1);
}

int main() 
{

	g_mainWindow = Window(800, 600);
	g_mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	GLuint uniformProjection = 0, uniformModel = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)g_mainWindow.GetBufferWidth() / g_mainWindow.GetBufferHeight(), 0.1f, 100.0f);

	// Loop until window closed
	while (!g_mainWindow.GetShouldClose())
	{
		// Get + Handle User Input
		glfwPollEvents();

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		g_shaderList[0].UseShader();
		uniformModel = g_shaderList[0].GetModelLocation();
		uniformProjection = g_shaderList[0].GetProjectionLocation();

		glm::mat4 model;	

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		g_meshList[0]->RenderMesh();

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		g_meshList[1]->RenderMesh();

		glUseProgram(0);

		g_mainWindow.SwapBuffers();
	}

	return 0;
}