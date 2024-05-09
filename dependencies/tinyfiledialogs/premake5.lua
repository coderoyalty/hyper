project "TinyFileDialogs"
	kind "StaticLib"
	language "C"
	targetdir ("%{wks.location}/bin/%{wks.name}/%{cfg.longname}")
	objdir ("%{wks.location}/bin-int/%{wks.name}/%{cfg.longname}")

	files
	{
		"tinyfiledialogs.c",
		"tinyfiledialogs.h"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}
	
	flags {"NoPCH"}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"


	filter "configurations:Release"
		runtime "Release"
		optimize "on"
