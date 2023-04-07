#pragma once 

#include "glm/glm.hpp"
#include "../geometry/Ray.h"

//TODO: address numerical drift

class Camera {

public:

	Camera(glm::vec3 position, glm::vec3 focalPoint, glm::vec3 up, float fovy);

	void TurnUp(int delta);
	void TurnRight(int delta);
	void ZoomIn(int delta);
	void PanUp(int delta);
	void PanRight(int delta);

	Ray GetRayAtPixel(int x, int y) const;

	glm::mat4 GetViewProj() const;

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