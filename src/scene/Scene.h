#pragma once

#include <unordered_map>
#include <memory>

#include "Camera.h"
#include "../geometry/Renderable.h"
#include "ConstructionPlane.h"

class Renderable;

class Scene {

public:

	static void Init();
	static void Destroy();

	static void Render();

	inline static Camera* GetCamera() { return m_Camera.get(); }

private:

	inline static std::unique_ptr<Camera> m_Camera;
	inline static std::unordered_map<unsigned, std::unique_ptr<Renderable>> m_Contents;
	inline static unsigned m_IDGenerator = 420U;
	inline static std::unique_ptr<ConstructionPlane> m_ConstructionPlane;

};