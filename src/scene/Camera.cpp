#pragma once

#include "Camera.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"

#include "../Window.h"

#include <iostream>

Camera::Camera(glm::vec3 position, glm::vec3 focalPoint, glm::vec3 up, float fovy)
	: m_Position(position), m_FocalPoint(focalPoint), m_Up(up), m_Fovy(fovy)
{ }

glm::mat4 Camera::GetViewProj() const {
	return glm::perspective(m_Fovy, Window::GetAspect(), m_NearPlane, m_FarPlane) * glm::lookAt(m_Position, m_FocalPoint, m_Up);
}

void Camera::TurnUp(int delta)
{

	glm::vec3 forward = m_FocalPoint - m_Position;
	glm::vec3 right = glm::cross(m_Up, forward);

	m_Position -= m_FocalPoint;
	m_Position = glm::rotate(m_Position, m_SensitivityRotation * delta, right);
	m_Position += m_FocalPoint;
	
	m_Up = glm::rotate(m_Up, m_SensitivityRotation * delta, right);
}

void Camera::TurnRight(int delta)
{

	m_Position -= m_FocalPoint;
	m_Position = glm::rotate(m_Position, m_SensitivityRotation * delta, {0.0,0.0,-1.0});
	m_Position += m_FocalPoint;

	m_Up = glm::rotate(m_Up, m_SensitivityRotation * delta, {0.0,0.0,-1.0});
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

	glm::vec3 forward = m_FocalPoint - m_Position;
	glm::vec3 right = glm::cross(m_Up, forward);
	glm::vec3 up = glm::normalize(glm::cross(forward, right));

	glm::vec3 translation = m_SensitivityPan * delta * dist * up;
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

Ray Camera::GetRayAtPixel(int x, int y) const 
{
	glm::vec3 forward = glm::normalize(m_FocalPoint - m_Position);
	glm::vec3 center = m_Position + forward;

	//theta/2 = tan(opp/2)
	//opp/2 = arctan(theta/2)
	//could cache size
	auto[xRes, yRes] = Window::GetSize();

	float sizeY = 2 * glm::atan(m_Fovy / 2);
	float sizeX = sizeY / yRes * xRes;

	//todo


}

