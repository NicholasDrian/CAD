#pragma once

#include "ColoredShader.h"
#include "../OpenGLUtils.h"

ColoredShader::ColoredShader() 
{
	Init("src/render/shaders/shaders/default.vert", "src/render/shaders/shaders/default.frag");
}