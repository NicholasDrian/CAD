#pragma once

class Window;
class Scene;

class Application {

public:

	Application();
	~Application();

private:

	Window* m_Window;
	Scene* m_Scene;

};