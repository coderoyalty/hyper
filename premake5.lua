include "dependencies.lua"
workspace "Hyper"
	architecture "x64"
	startproject "sandbox"

	configurations
	{
		"Debug", "Release"
	}

	flags
	{
		"MultiProcessorCompile"
	}

DependDir = "dependencies/"

group "Dependencies"
	include "dependencies/glad"
	include "dependencies/glfw"
	include "dependencies/imgui"
	include "dependencies/freetype"
	include "dependencies/yaml-cpp"
	include "dependencies/lua"
	include "dependencies/tinyfiledialogs"
	include "dependencies/box2d"

group "Hyper"
	include "engine"
	include "sandbox"
	include "sandbox-editor"
	include "hyper-pong"
