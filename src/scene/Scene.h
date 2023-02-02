#pragma once

#include <memory>
#include <vector>

#include "Camera.h"

class Renderable;

class Scene {

public:

	static void Init();
	static void Destroy();

	static void Render();

	inline static Camera* GetCamera() { return m_Camera; }

private:

	static Camera* m_Camera;

	static std::vector<std::unique_ptr<Renderable>> m_Contents;

};