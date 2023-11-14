#include "Shader.h"

Shader::Shader()
{
	shaderID = 0;
	uniformModel = 0;
	uniformProjection = 0;
}

void Shader::CreateFromString(const char* a_vertexCode, const char* fragmentCode)
{
	M_CompileShader(a_vertexCode, fragmentCode);
}

void Shader::CreateFromFiles(const char* a_vertexLocation, const char* a_fragmentLocation)
{
	std::string vertexString = ReadFile(a_vertexLocation);
	std::string fragmentString = ReadFile(a_fragmentLocation);
	const char* vertexCode = vertexString.c_str();
	const char* fragmentCode = fragmentString.c_str();

	M_CompileShader(vertexCode, fragmentCode);
}

std::string Shader::ReadFile(const char* a_fileLocation)
{
	std::string content;
	std::ifstream fileStream(a_fileLocation, std::ios::in);

	if (!fileStream.is_open()) {
		printf("Failed to read %s! File doesn't exist.", a_fileLocation);
		return "";
	}

	std::string line = "";
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}

void Shader::M_CompileShader(const char* a_vertexCode, const char* a_fragmentCode)
{
	shaderID = glCreateProgram();

	if (!shaderID)
	{
		printf("Error creating shader program!\n");
		return;
	}

	M_AddShader(shaderID, a_vertexCode, GL_VERTEX_SHADER);
	M_AddShader(shaderID, a_fragmentCode, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shaderID);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error linking program: '%s'\n", eLog);
		return;
	}

	glValidateProgram(shaderID);
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error validating program: '%s'\n", eLog);
		return;
	}

	uniformProjection = glGetUniformLocation(shaderID, "projection");
	uniformModel = glGetUniformLocation(shaderID, "model");
}

GLuint Shader::GetProjectionLocation()
{
	return uniformProjection;
}
GLuint Shader::GetModelLocation()
{
	return uniformModel;
}

void Shader::UseShader()
{
	glUseProgram(shaderID);
}

void Shader::ClearShader()
{
	if (shaderID != 0)
	{
		glDeleteProgram(shaderID);
		shaderID = 0;
	}

	uniformModel = 0;
	uniformProjection = 0;
}


void Shader::M_AddShader(GLuint a_program, const char* a_shaderCode, const GLenum shaderType)
{
	GLuint shaderProgram = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = a_shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(a_shaderCode);

	glShaderSource(shaderProgram, 1, theCode, codeLength);
	glCompileShader(shaderProgram);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(shaderProgram, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(shaderProgram, sizeof(eLog), NULL, eLog);
		printf("Error compiling the %d shader: '%s'\n", shaderType, eLog);
		return;
	}

	glAttachShader(a_program, shaderProgram);
}

Shader::~Shader()
{
	ClearShader();
}
