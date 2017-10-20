#ifndef DUKDEMO_INCLUDE__DUKDEMO__RENDER__UTIL__H
#define DUKDEMO_INCLUDE__DUKDEMO__RENDER__UTIL__H
#include <string>
#include <GL/glew.h>


namespace dukdemo {
namespace render {


/** Create a simple GL program. */
GLuint const createProgram();


/** Get the info log of an OpenGL object. */
std::string
getLog(
	GLuint const handle,
	decltype(glGetShaderiv) writeLength,
	decltype(glGetShaderInfoLog) writeLog
);


/** Get the info log of a shader. */
inline std::string
getShaderLog(GLuint const handle)
{
	return getLog(handle, glGetShaderiv, glGetShaderInfoLog);
}


/** Get the info log of a program. */
inline std::string
getProgramLog(GLuint const handle)
{
	return getLog(handle, glGetProgramiv, glGetProgramInfoLog);
}


/**
 * Compile a shader.
 *
 * @returns the shader handle if successful, or 0 on failure.
 */
GLuint const compileShader(GLuint const type, GLchar const* pSource);


/**
 * Check for GL errors.
 *
 * If any errors are found, they are logged then an exception thrown.
 * @throw std::runtime_error
 */
void checkGLErrors(char const* const pErrorContext);


} // namespace render
} // namespace dukdemo
#endif // #ifndef DUKDEMO_INCLUDE__DUKDEMO__RENDER__UTIL__H
