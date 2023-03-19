#pragma once

#include <memory>
#include <vector>

#include "Camera.h"
#include "../geometry/Renderable.h"

class Renderable;

class Scene {

public:

	static void Init();
	static void Destroy();

	static void Render();

	inline static Camera* GetCamera() { return m_Camera; }

private:

	inline static Camera* m_Camera;

	inline static std::vector<std::unique_ptr<Renderable>> m_Contents;

};