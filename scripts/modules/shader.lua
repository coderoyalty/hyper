---@diagnostic disable: unused-local, missing-return

---@class Shader
local Shader = {
  --- Create a new shader.
  ---@param shaderType number  The type of shader to create (VERTEX or FRAGMENT).
  ---@return Shader
  new = function(shaderType)

  end
}

--- Create a new shader.
---@param shaderType ShaderType  The type of shader to create (VERTEX or FRAGMENT).
---@return Shader
function Shader:new(shaderType)

end

--- Compile a shader from source.
--- @param shaderSource string The shader source code to compile
function Shader:compile(shaderSource)

end

return Shader
