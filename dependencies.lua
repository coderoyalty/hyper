includes = {}

includes["GLFW"] = "%{wks.location}/dependencies/glad/include"
includes["GLAD"] = "%{wks.location}/dependencies/glfw/include"
includes["IMGUI"] = "%{wks.location}/dependencies/imgui"
includes["IMGUIZMO"] = "%{wks.location}/dependencies/imguizmo"
includes["FREETYPE"] = "%{wks.location}/dependencies/freetype/include"
includes["YAML_CPP"] = "%{wks.location}/dependencies/yaml-cpp/include"
includes["LUA"] = "%{wks.location}/dependencies/lua/src"
includes["SOL"] = "%{wks.location}/dependencies/sol/include"
includes["ENTT"] = "%{wks.location}/dependencies/vendor/entt/include"

-- vendor
includes["GLM"] = "%{wks.location}/dependencies/vendor/glm/include"
includes["STB"] = "%{wks.location}/dependencies/vendor/stb"
