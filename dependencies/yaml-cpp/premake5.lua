project "YAML-CPP"
	kind "StaticLib"
  	language "C++"
  	cppdialect "C++17"
  	targetdir ("%{wks.location}/bin/%{wks.name}/%{cfg.longname}")
  	objdir ("%{wks.location}/bin/%{wks.name}/%{cfg.longname}")
	staticruntime "off"

	files
	{
		"src/**.h",
		"src/**.cpp",

		"include/**.h"
	}

	includedirs
	{
		"include"
	}

	defines {
		"YAML_CPP_STATIC_DEFINE",
	}

	
	flags {"NoPCH"}

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
