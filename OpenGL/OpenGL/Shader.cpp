#include "Shader.h"

Shader::Shader()
{
	m_shaderID = 0;
	m_uniformModel = 0;
	m_uniformProjection = 0;
}

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode)
{
	M_CompileShader(vertexCode, fragmentCode);
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
	m_shaderID = glCreateProgram();

	if (!m_shaderID)
	{
		printf("Error creating shader program!\n");
		return;
	}

	M_AddShader(m_shaderID, a_vertexCode, GL_VERTEX_SHADER);
	M_AddShader(m_shaderID, a_fragmentCode, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(m_shaderID);
	glGetProgramiv(m_shaderID, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(m_shaderID, sizeof(eLog), NULL, eLog);
		printf("Error linking program: '%s'\n", eLog);
		return;
	}

	glValidateProgram(m_shaderID);
	glGetProgramiv(m_shaderID, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(m_shaderID, sizeof(eLog), NULL, eLog);
		printf("Error validating program: '%s'\n", eLog);
		return;
	}

	m_uniformProjection = glGetUniformLocation(m_shaderID, "projection");
	m_uniformModel = glGetUniformLocation(m_shaderID, "model");
}

GLuint Shader::GetProjectionLocation()
{
	return m_uniformProjection;
}
GLuint Shader::GetModelLocation()
{
	return m_uniformModel;
}

void Shader::UseShader()
{
	glUseProgram(m_shaderID);
}

void Shader::ClearShader()
{
	if (m_shaderID != 0)
	{
		glDeleteProgram(m_shaderID);
		m_shaderID = 0;
	}

	m_uniformModel = 0;
	m_uniformProjection = 0;
}


void Shader::M_AddShader(GLuint a_theProgram, const char* a_shaderCode, const GLenum a_shaderType)
{
	GLuint Shader = glCreateShader(a_shaderType);

	const GLchar* theCode[1];
	theCode[0] = a_shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(a_shaderCode);

	glShaderSource(Shader, 1, theCode, codeLength);
	glCompileShader(Shader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(Shader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(Shader, sizeof(eLog), NULL, eLog);
		printf("Error compiling the %d shader: '%s'\n", a_shaderType, eLog);
		return;
	}

	glAttachShader(a_theProgram, Shader);
}

Shader::~Shader()
{
	ClearShader();
}
