#include "Shader.h"

Shader::Shader()
	:program(NULL),
	uModel(NULL),
	uProjection(NULL),
	uView(NULL),
	uTextureUnit(NULL)
{
}

Shader::~Shader()
{
	glDeleteProgram(program);
}

std::string Shader::load(const char* path,uint32_t shaderType)
{
	std::ifstream file(path);
	std::ostringstream sourceCode;
	if (!file)
	{
		std::cout << "ERROR::CAN_NOT_OPEN_FILE \"" <<path<<"\""<< std::endl;
		exit(1);
	}
	else
	{
		
		sourceCode << file.rdbuf();
	}

	file.close();
	return sourceCode.str();
}

uint32_t Shader::compile(const char* sourceCode, uint32_t shaderType)
{
	uint32_t shaderID = glCreateShader(shaderType);
	glShaderSource(shaderID, 1, &sourceCode, NULL);
	glCompileShader(shaderID);

	int32_t compilationResult;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compilationResult);

	if (compilationResult == GL_FALSE)
	{
		int length;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);
		char* errLog = new char[length];
		glGetShaderInfoLog(shaderID, length, &length, errLog);
		std::cout << "ERROR::COMPILE:" << ((shaderType == GL_VERTEX_SHADER) ? "VERTEX_SHADER: " : "FRAGMENT_SHADER: ") << errLog << std::endl;
		delete[] errLog;
		glDeleteShader(shaderID);
		return 0;
	}

	return shaderID;

}

void Shader::attachAndLink(uint32_t vShaderID, uint32_t fShaderID)
{
	program = glCreateProgram();
	glAttachShader(program, vShaderID);
	glAttachShader(program, fShaderID);
	glLinkProgram(program);

	int32_t linkResult;
	glGetProgramiv(program, GL_LINK_STATUS, &linkResult);

	if (linkResult == GL_FALSE)
	{
		int length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
		char* errLog = new char[length];
		glGetProgramInfoLog(program, length, &length, errLog);
		std::cout << "ERROR::LINK:" << errLog << std::endl;
		delete[] errLog;
		glDeleteProgram(program);
	}

	//glValidateProgram(program);
	glDetachShader(program, vShaderID);
	glDetachShader(program, fShaderID);
	glDeleteShader(vShaderID);
	glDeleteShader(fShaderID);
}

void Shader::create(const char* vertexShaderPath, const char* fragmentShaderPath)
{
	std::string vertexShader = load(vertexShaderPath, GL_VERTEX_SHADER);
	std::string fragmentShader = load(fragmentShaderPath, GL_FRAGMENT_SHADER);
	uint32_t vShaderID = this->compile(vertexShader.c_str(), GL_VERTEX_SHADER);
	uint32_t fShaderID = this->compile(fragmentShader.c_str(), GL_FRAGMENT_SHADER);
	this->attachAndLink(vShaderID, fShaderID);
	glUseProgram(program);
}

uint32_t Shader::getProgram() const
{
	return program;
}
