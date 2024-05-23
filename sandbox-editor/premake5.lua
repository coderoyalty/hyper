project "hyper-editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	targetdir ("%{wks.location}/bin/%{wks.name}/%{cfg.longname}")
	objdir ("%{wks.location}/bin-int/%{wks.name}/%{cfg.longname}")

	files
	{
		"**.hpp", "**.cpp", "*.c"
	}


	defines {
		"HYPER_STATIC_EXPORTS",
		"YAML_CPP_STATIC_DEFINE"
	}

	includedirs
	{
		"%{wks.location}/engine/src",
		"%{includes.GLFW}",
		"%{includes.GLAD}",
		"%{includes.GLM}",
		"%{includes.STB}",
		"%{includes.IMGUI}",
		"%{includes.ENTT}",
		"%{includes.IMGUIZMO}",
		"%{includes.YAML_CPP}",
		"%{includes.LUA}",
		"%{includes.SOL}",
		"%{includes.BOX2D}"
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