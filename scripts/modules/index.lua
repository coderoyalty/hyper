local ShaderProgram = require "scripts.modules.shader_program"
local Shader = require "scripts.modules.shader"
local ShaderType = require "scripts.modules.shader_type"
if hyp then
  return hyp
end

local hyp = {
  ShaderProgram = ShaderProgram,
  Shader = Shader,
  ShaderType = ShaderType,
}

return hyp
