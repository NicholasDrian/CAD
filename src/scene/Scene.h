#pragma once

#include <memory>

#include "Camera.h"

class Scene {

public:

	static void Init();
	static void Destroy();

	static void Render();

private:

	static Camera* m_Camera;

};