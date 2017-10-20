#include <stdexcept>

#include <GL/glew.h>

#include <easylogging++.h>

#include "dukdemo/render/util.h"


namespace dukdemo {
namespace render {


/**
 * Create a GL program and add compiled shaders.
 *
 * @returns the ID of the created program.
 */
GLuint const createProgram()
{
	// Create OpenGL shaders and program.
	GLuint const vertShaderID{compileShader(GL_VERTEX_SHADER, R"GLS(\
#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec4 colour;

out vec4 fsColour;
uniform mat4 MVP;


void main() {
	gl_Position =  MVP * vec4(position.x, position.y, 0, 1);
	fsColour = colour;
}

	)GLS")};

	GLuint const fragShaderID{compileShader(GL_FRAGMENT_SHADER, R"GLS(\
#version 330 core

in vec4 fsColour;
out vec4 fragColour;

void main() {
	fragColour = fsColour;
}
	)GLS")};

	GLuint const programID{glCreateProgram()};
	glAttachShader(programID, vertShaderID);
	glAttachShader(programID, fragShaderID);
	glLinkProgram(programID);
	{
		GLint success{GL_FALSE};
		glGetProgramiv(programID, GL_LINK_STATUS, &success);
		if (success != GL_TRUE)
		{
			LOG(ERROR)
				<< "Failed to link program: "
				<< getProgramLog(programID);
			throw std::runtime_error{"Program link failed"};
		}
	}

	return programID;
}


/** Get the log of a shader or program. */
std::string
getLog(
	GLuint const handle,
	decltype(glGetShaderiv) writeLength,
	decltype(glGetShaderInfoLog) writeLog
)
{
	GLint length{0};
	writeLength(handle, GL_INFO_LOG_LENGTH, &length);
	std::string log(length, '\0');
	if (length > 0)
	{
		writeLog(handle, length, nullptr, &log[0]);
	}
	return log;
}


/** Compile a shader. */
GLuint const compileShader(GLuint const type, GLchar const* pSource)
{
	GLuint const shaderID{glCreateShader(type)};
	glShaderSource(shaderID, 1, &pSource, nullptr);
	glCompileShader(shaderID);

	GLint compiled{GL_FALSE};
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compiled);
	if (compiled != GL_TRUE)
	{
		LOG(ERROR) << "Shader compilation failed: " << getShaderLog(shaderID);
		throw std::runtime_error{"Shader compilation failed"};
	}

	return shaderID;
}


void checkGLErrors(char const* const pErrorContext)
{
	bool foundErrors{false};
	decltype(glGetError()) error;
	while ((error = glGetError()))
	{
		LOG(ERROR)
			<< '[' << pErrorContext << "] "
			<< "GL error (" << error << "): "
			<< glewGetErrorString(error);
		foundErrors = true;
	}
	if (foundErrors)
	{
		throw std::runtime_error{"GL errors detected"};
	}
}


} // namespace render
} // namespace dukdemo
