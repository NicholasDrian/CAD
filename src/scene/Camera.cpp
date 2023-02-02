#pragma once

#include "Camera.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"

#include "../Window.h"

#include "stdexcept"
#include <iostream>

Camera::Camera(glm::vec3 position, glm::vec3 focalPoint, glm::vec3 up, float fovy)
	: m_Position(position), m_FocalPoint(focalPoint), m_Up(up), m_Fovy(fovy)
{
}

glm::mat4 Camera::GetViewProj() const {
	return glm::perspective(m_Fovy, Window::GetAspect(), 0.01f, 10000.0f) * glm::lookAt(m_Position, m_FocalPoint, m_Up);
}

void Camera::LoopUp(int delta)
{
	glm::vec3 forward = m_FocalPoint - m_Position;
	glm::vec3 right = glm::cross(forward, m_Up);
	m_FocalPoint = glm::rotate(m_FocalPoint, m_SensitivityRotation * delta, right);
	forward = m_FocalPoint - m_Position;
	m_Up = glm::normalize(glm::cross(forward, right));
}

void Camera::Zoom(int delta) {

}

void Camera::LookRight(int delta)
{
}
