#pragma once 

#include "glm/glm.hpp"
#include "../geometry/Ray.h"

//TODO: address numerical drift

class Camera {

public:

	Camera(glm::vec3 position, glm::vec3 focalPoint, float fovy);

	void TurnUp(int delta);
	void TurnRight(int delta);
	void ZoomIn(int delta);
	inline void PanUp(int delta) { PanUp((float)delta); };
	void PanUp(float delta);
	inline void PanRight(int delta) { PanRight((float)delta); }
	void PanRight(float delta);
	void MoveForward(float delta);

	void RepositionFocal(const glm::vec3& newFocal);

	Ray GetRayAtPixel(int x, int y) const;

	float GetPixelSizeAtPoint(const glm::vec3& point) const;

	glm::mat4 GetViewProj() const;
	glm::mat4 GetView() const;
	glm::mat4 GetProj() const;
	glm::vec3 GetForward() const;

	inline glm::vec3 GetPosition() const { return m_Position; }

	static float GetNearPlane() { return m_NearPlane; }
	static float GetFarPlane() { return m_FarPlane; }

private:

	glm::vec3 m_Position, m_FocalPoint, m_Up;
	float m_Fovy;

	inline static const float m_SensitivityRotation = 0.005f;
	inline static const float m_SensitivityZoom = 0.996f;
	inline static const float m_SensitivityPan = 0.003f;
	
	inline static const float m_NearPlane = 1.0f;
	inline static const float m_FarPlane = 1000.0f;

};