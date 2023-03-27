#pragma once

#include "ColoredLineShader.h"
#include "../OpenGLUtils.h"

ColoredLineShader::ColoredLineShader()
{
	Init("src/render/shaders/shaders/ColoredLine.vert", "src/render/shaders/shaders/ColoredLine.frag");
}