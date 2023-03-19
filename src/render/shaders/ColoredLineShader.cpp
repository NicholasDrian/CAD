#pragma once

#include "ColoredLineShader.h"
#include "../OpenGLUtils.h"

ColoredLineShader::ColoredLineShader()
{
	Init("src/render/shaders/shaders/ColoredTri.vert", "src/render/shaders/shaders/ColoredTri.frag");
}