workspace "CAD"
	architecture "x64"
	configurations { "Debug", "Release" }

project "CAD"
	
	kind "consoleApp"
	language "C++"
	targetdir "build/bin/%{cfg.architecture}-%{cfg.buildcfg}"
	objdir "build/bin-int/%{cfg.architecture}-%{cfg.buildcfg}"
	cppdialect "C++17"

	files {
		"src/**.h", 
		"src/**.cpp",
		"src/**.vert",
		"src/**.frag",
		"dependencies/imgui/*.cpp",
		"dependencies/glm/**.hpp",
		"dependencies/glm/**.inl"
	}

	includedirs {
		"dependencies/glfw/include",
		"dependencies/glew/include",
		"dependencies/imgui",
		"dependencies/glm"
	}

	links {
		"Opengl32",
		"Glu32",
		"dependencies/glew/lib/glew32s",
		"dependencies/glfw/lib-vc2022/glfw3"
	}

	ignoredefaultlibraries { 
		"MSVCRT",
		"LIBCMT"
	}

	defines { 
		"GLEW_STATIC" 
	}

	filter "configurations:Debug"
		defines { "CAD_DEBUG" }
		symbols "On"

	filter "configurations:Release"
		defines { "CAD_RELEASE" }
		optimize "On"