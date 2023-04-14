#pragma once

#include <unordered_map>
#include <unordered_set>
#include <memory>

#include "Camera.h"
#include "../geometry/Renderable.h"
#include "../gui/AffineTransformWidget.h"
#include "ConstructionPlane.h"

#include "glm/glm.hpp"

class Renderable;

class Scene {

public:

	static void Init();
	static void Destroy();

	static void Render();
	static void DrawGUI();

	static void AddToScene(std::unique_ptr<Renderable> obj);

	static void HandleClick(int x, int y, int button, int mods);

	static void DeleteSelection();
	static void Delete(unsigned id);
	static const glm::mat4& GetSelectionTransform();
	static AxisAlignedBoundingBox GetSelectedBoundingBox();

	static bool IntersectScene(int x, int y, glm::vec3& outPoint);

	inline static unsigned GetNewID() { return m_IDGenerator++; }

	inline static Camera* GetCamera() { return m_Camera.get(); }

	static void ClearSelection();

private:

	inline static const float m_MaxExtend = 10000.0f;

	inline static std::unique_ptr<AffineTransformWidget> m_TransformWidget;

	inline static std::unordered_set<uint32_t> m_Selected;
	inline static std::unordered_set<uint32_t> m_SubSelected;

	inline static std::unique_ptr<Camera> m_Camera;
	inline static std::unordered_map<unsigned, std::unique_ptr<Renderable>> m_Contents;
	inline static unsigned m_IDGenerator = 420U;
	inline static std::unique_ptr<ConstructionPlane> m_ConstructionPlane;

};