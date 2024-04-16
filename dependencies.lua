includes = {}

includes["GLFW"] = "%{wks.location}/dependencies/glad/include"
includes["GLAD"] = "%{wks.location}/dependencies/glfw/include"
includes["IMGUI"] = "%{wks.location}/dependencies/imgui"
includes["IMGUIZMO"] = "%{wks.location}/dependencies/imguizmo"
includes["FREETYPE"] = "%{wks.location}/dependencies/freetype/include"

-- vendor
includes["GLM"] = "%{wks.location}/dependencies/vendor/glm/include"
includes["STB"] = "%{wks.location}/dependencies/vendor/stb"
includes["SOL"] = "%{wks.location}/dependencies/vendor/sol"
includes["ENTT"] = "%{wks.location}/dependencies/vendor/entt"
