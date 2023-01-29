#pragma once 

#include "Scene.h"

#include "Camera.h"

Scene::Scene() 
	: m_Camera(new Camera())
{
}

Scene::~Scene()
{
	delete m_Camera;
}
