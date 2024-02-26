---@diagnostic disable: unused-local, missing-return
--- Module definition for ShaderProgram.
--- This module provides a Lua interface for ShaderProgram functionality.
--- If ShaderProgram is already defined, this module returns the existing definition.


---@class ShaderProgram
local ShaderProgram = {
  --- Creates a new ShaderProgram instance.
  --- @return ShaderProgram The newly created ShaderProgram instance.
  new = function()
  end
}

--- Attaches a shader to the shader program.
--- @param Shader Shader object to attach.
function ShaderProgram:attachShader(Shader)
end

--- Sets this shader program to be the active one.
function ShaderProgram:use()
end

--- Links the shader program.
function ShaderProgram:link()
end

return ShaderProgram -- Export ShaderProgram module
