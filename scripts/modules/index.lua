local ShaderProgram = require "scripts.modules.shader_program"
local Shader = require "scripts.modules.shader"
local ShaderType = require "scripts.modules.shader_type"
if wow then
  return wow
end

local wow = {
  ShaderProgram = ShaderProgram,
  Shader = Shader,
  ShaderType = ShaderType,
}

return wow
