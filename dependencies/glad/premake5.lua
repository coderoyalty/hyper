project "GLAD"
	kind "StaticLib"
	language "C"
	targetdir ("%{wks.location}/bin/%{wks.name}/%{cfg.longname}")
	objdir ("%{wks.location}/bin-int/%{wks.name}/%{cfg.longname}")

	files
	{
		"include/**.h",
		"src/**.c"
	}

	includedirs
	{
		"include"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"