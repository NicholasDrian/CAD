#pragma once

#include <unordered_map>
#include <unordered_set>
#include <memory>

#include "Camera.h"
#include "../geometry/Renderable.h"
#include "ConstructionPlane.h"

#include "glm/glm.hpp"

class Renderable;

class Scene {

public:

	static void Init();
	static void Destroy();

	static void Render();

	static void AddToScene(std::unique_ptr<Renderable> obj);

	static void HandleClick(int x, int y, int button, int mods);

	static void DeleteSelection();
	static void Delete(unsigned id);

	static bool IntersectScene(int x, int y, glm::vec3& outPoint);

	inline static unsigned GetNewID() { return m_IDGenerator++; }

	inline static Camera* GetCamera() { return m_Camera.get(); }

	inline static void ClearSelection() { m_Selected.clear(); }

private:

	inline static const float m_MaxExtend = 10000.0f;

	inline static std::unordered_set<unsigned> m_Selected;
	inline static std::unique_ptr<Camera> m_Camera;
	inline static std::unordered_map<unsigned, std::unique_ptr<Renderable>> m_Contents;
	inline static unsigned m_IDGenerator = 420U;
	inline static std::unique_ptr<ConstructionPlane> m_ConstructionPlane;

};