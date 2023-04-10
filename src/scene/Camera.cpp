#pragma once

#include "Camera.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"

#include "../Window.h"

#include <iostream>

Camera::Camera(glm::vec3 position, glm::vec3 focalPoint, float fovy)
	: m_Position(position), m_FocalPoint(focalPoint), m_Fovy(fovy)
{ 
	glm::vec3 forward = m_FocalPoint - m_Position;
	glm::vec3 right = glm::cross(forward, { 0.0f,0.0f,1.0f });
	m_Up = glm::normalize(glm::cross(right, forward));
}

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

void Camera::PanUp(float delta)
{
	float dist = glm::distance(m_FocalPoint, m_Position);
	glm::vec3 translation = m_SensitivityPan * delta * dist * m_Up;
	m_Position += translation;
	m_FocalPoint += translation;
}

void Camera::PanRight(float delta)
{
	glm::vec3 forward = m_FocalPoint - m_Position;
	glm::vec3 right = glm::cross(m_Up, forward);
	glm::vec3 translation = m_SensitivityPan * delta * right;
	m_Position += translation;
	m_FocalPoint += translation;
}

void Camera::MoveForward(float delta) {
	glm::vec3 forward = m_FocalPoint - m_Position;
	glm::vec3 translation = m_SensitivityPan * delta * forward;
	m_Position += translation;
	m_FocalPoint += translation;
}

void Camera::RepositionFocal(const glm::vec3& newFocal)
{
	glm::vec3 delta = newFocal - m_FocalPoint;
	m_Position += delta;
	m_FocalPoint += delta;
}

Ray Camera::GetRayAtPixel(int x, int y) const 
{
	glm::vec3 forward = glm::normalize(m_FocalPoint - m_Position);
	glm::vec3 center = m_Position + forward;
	glm::vec3 right = glm::cross(forward, m_Up);

	//could cache size on screen resize
	auto [xRes, yRes] = Window::GetSize();
	float sizeY = 2.0f * glm::tan(m_Fovy / 2.0f);
	float sizeX = sizeY / yRes * xRes;

	glm::vec3 screenPoint = center 
		+ right * (sizeX / xRes * (x - xRes / 2.0f)) 
		- m_Up * (sizeY / yRes * (y - yRes / 2.0f));

	return Ray(m_Position, glm::normalize(screenPoint - m_Position));

}

