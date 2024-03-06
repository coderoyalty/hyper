---@diagnostic disable: undefined-global, lowercase-global
local vertexShaderSource = [[
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aColor;

    out vec4 outColor;
    void main()
    {
        gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
        outColor = vec4(aColor, 1.0);
    }
]]

local fragmentShaderSource = [[
    #version 330 core
    out vec4 FragColor;
    in vec4 outColor;
    void main()
    {
        FragColor = outColor;
    }
]]

hyp = require "scripts.modules.index"

---@types ShaderProgram
local program = hyp.ShaderProgram.new("scripts/vertex.vert", "scripts/fragment.frag")

program:link()

-- this function will be called in the main loop
function main()
    program:use()
end
