#pragma once

#include "GL/glew.h"

void PrintOpenGLErrors(char const* const Function, char const* const File, int const Line);

void PrintShaderInfoLog(GLint const shader);

void PrintProgramInfoLog(GLint const program);

#ifdef CAD_DEBUG
#	define GLCall(x) do { PrintOpenGLErrors(">>BEFORE<< "#x, __FILE__, __LINE__);(x); PrintOpenGLErrors(#x, __FILE__, __LINE__); } while (0)
#	define GLResult(x) (x); PrintOpenGLErrors(#x, __FILE__, __LINE__);
#else
#	define GLCall(x) (x)
#	define GLResult(x) (x);
#endif


//extern const char* glsl_version;