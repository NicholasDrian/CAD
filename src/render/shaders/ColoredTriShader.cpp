#pragma once

#include "ColoredTriShader.h"
#include "../OpenGLUtils.h"

ColoredTriShader::ColoredTriShader() 
{
	Init("src/render/shaders/shaders/ColoredTri.vert", "src/render/shaders/shaders/ColoredTri.frag");
}