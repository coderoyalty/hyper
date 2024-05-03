project "LUA"
	kind "StaticLib"
  	language "C++"
  	cppdialect "C++17"
  	targetdir ("%{wks.location}/bin/%{wks.name}/%{cfg.longname}")
  	objdir ("%{wks.location}/bin/%{wks.name}/%{cfg.longname}")
	staticruntime "on"

	files
	{
		"src/**.h",
		"src/**.c",
		"{includes.SOL}/**.hpp"
	}

	includedirs
	{
		"src/",
		"{includes.SOL}"
	}

	excludes {
		"src/luac.c"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
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
