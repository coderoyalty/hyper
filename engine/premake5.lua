project "Hyper"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	targetdir ("%{wks.location}/bin/%{wks.name}/%{cfg.longname}")
	objdir ("%{wks.location}/bin-int/%{wks.name}/%{cfg.longname}")

	pchheader "pch.h"
	pchsource "src/pch.cpp"

	files
	{
		"src/**.h",
		"src/**.hpp", "src/**.cpp",
		"../dependencies/vendor/stb/**.cpp",

		"../dependencies/imguizmo/ImGuizmo.h",
		"../dependencies/imguizmo/ImGuizmo.cpp"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS", "HYPER_STATIC_EXPORTS",
		"GLFW_INCLUDE_NONE",
		"YAML_CPP_STATIC_DEFINE"
	}

	includedirs
	{
    	"src/",
		"%{includes.GLFW}",
		"%{includes.GLAD}",
		"%{includes.GLM}",
		"%{includes.STB}",
		"%{includes.IMGUI}",
		"%{includes.ENTT}",
		"%{includes.IMGUIZMO}",
		"%{includes.FREETYPE}",
		"%{includes.YAML_CPP}",
		"%{includes.LUA}",
		"%{includes.SOL}",
		"%{includes.FILE_DIALOGS}",
		"%{includes.BOX2D}"
	}
	
	links
	{
		"GLFW", "GLAD", "IMGUI", "opengl32.lib", "FREETYPE",
		"YAML-CPP", "LUA",
		"TinyFileDialogs",
		"Box2D"
	}

	filter "files:vendor/imguizmo/**.cpp"
	flags {"NoPCH"}

	filter "system:windows"
		systemversion "latest"

		links
		{
			"%{library.WinSock}",
			"%{library.WinMM}",
			"%{library.WinVersion}",
			"%{library.BCrypt}",
		}

	filter "configurations:Debug"
		defines {"HYPER_DEBUG", "HYPER_ASSERTION_ENABLED"}
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "HYPER_RELEASE"
		runtime "Release"
		optimize "on"

	filter "action:vs*"
    buildoptions { "/bigobj" }
