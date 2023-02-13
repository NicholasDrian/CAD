#pragma once

#include "Camera.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"

#include "../Window.h"

Camera::Camera(glm::vec3 position, glm::vec3 focalPoint, glm::vec3 up, float fovy)
	: m_Position(position), m_FocalPoint(focalPoint), m_Up(up), m_Fovy(fovy)
{ }

glm::mat4 Camera::GetViewProj() const {
	return glm::perspective(m_Fovy, Window::GetAspect(), 0.01f, 10000.0f) * glm::lookAt(m_Position, m_FocalPoint, m_Up);
}

void Camera::TurnUp(int delta)
{
	glm::vec3 forward = m_FocalPoint - m_Position;
	glm::vec3 right = glm::cross(forward, m_Up);
	m_FocalPoint -= m_Position;
	m_FocalPoint = glm::rotate(m_FocalPoint, m_SensitivityRotation * delta, right);
	m_FocalPoint += m_Position;
	forward = m_FocalPoint - m_Position;
	m_Up = glm::normalize(glm::cross(right, forward));
}

void Camera::TurnRight(int delta)
{
	glm::vec3 up = (m_Up.z > 0.0f) ? glm::vec3{ 0.0f, 0.0f, 1.0f } : glm::vec3{ 0.0f, 0.0f, -1.0f };
	m_FocalPoint -= m_Position;
	m_FocalPoint = glm::rotate(m_FocalPoint, m_SensitivityRotation * delta, up);
	m_FocalPoint += m_Position;
	m_Up = glm::rotate(m_Up, m_SensitivityRotation * delta, up);
}

void Camera::ZoomIn(int delta) 
{
	glm::vec3 forward = m_FocalPoint - m_Position;
	float distance = glm::length(forward);
	forward *= glm::pow(m_SensitivityZoom, delta);
	m_Position = m_FocalPoint - forward;
}

void Camera::PanUp(int delta)
{
	float dist = glm::distance(m_Position, m_FocalPoint);
	glm::vec3 translation = m_SensitivityPan * delta * dist * m_Up;
	m_Position += translation;
	m_FocalPoint += translation;
}

void Camera::PanRight(int delta)
{
	glm::vec3 forward = m_FocalPoint - m_Position;
	glm::vec3 right = glm::cross(m_Up, forward);
	glm::vec3 translation = m_SensitivityPan * delta * right;
	m_Position += translation;
	m_FocalPoint += translation;
}

