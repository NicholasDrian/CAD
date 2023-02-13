#pragma once 

#include "glm/glm.hpp"

class Camera {

public:

	Camera(glm::vec3 position, glm::vec3 focalPoint, glm::vec3 up, float fovy);

	void TurnUp(int delta);
	void TurnRight(int delta);
	void ZoomIn(int delta);
	void PanUp(int delta);
	void PanRight(int delta);

	glm::mat4 GetViewProj() const;

private:

	glm::vec3 m_Position, m_FocalPoint, m_Up;
	float m_Fovy;

	const float m_SensitivityRotation = 0.002f;
	const float m_SensitivityZoom = 0.996f;
	const float m_SensitivityPan = 0.003f;

};