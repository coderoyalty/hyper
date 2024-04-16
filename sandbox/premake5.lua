project "sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"
	targetdir ("%{wks.location}/bin/%{wks.name}/%{cfg.longname}")
	objdir ("%{wks.location}/bin-int/%{wks.name}/%{cfg.longname}")
	
	files
	{
		"**.hpp", "**.cpp", "*.c",
	}

	-- defines {	"GLFW_DLL"	}

	defines {	"HYPER_STATIC_EXPORTS"	}

	includedirs
	{
		"%{wks.location}/engine/src",
		"%{includes.GLFW}",
		"%{includes.GLAD}",
		"%{includes.GLM}",
		"%{includes.STB}",
		"%{includes.IMGUI}",
		"%{includes.ENTT}",
		"%{includes.IMGUIZMO",
		"%{includes.FREETYPE}"
	}
	links
	{
		"Hyper"
	}

	-- flags {"NoPCH"}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines {"HYPER_DEBUG", "HYPER_ASSERTION_ENABLED"}
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "HYPER_RELEASE"
		runtime "Release"
		optimize "on"