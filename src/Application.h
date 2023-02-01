#pragma once

#include <memory>

class Window;
class Scene;

class Application {

public:

	static void Run();
	static void Terminate();

private:

	static void Init();

};