local vertexShaderSource = [[
    #version 330 core
    layout (location = 0) in vec3 aPos;
    void main()
    {
        gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    }
]]

local fragmentShaderSource = [[
    #version 330 core
    out vec4 FragColor;
    void main()
    {
        FragColor = vec4(1.0f, 1.0f, 0.2f, 1.0f);
    }
]]

local vshader = Shader.new(ShaderType.VERTEX)
local fshader = Shader.new(ShaderType.FRAGMENT)

vshader:compile(vertexShaderSource)
fshader:compile(fragmentShaderSource)

local program = ShaderProgram.new()
program:attachShader(vshader)
program:attachShader(fshader)

program:link()

-- this function will be called in the main loop
function main()
  program:use()
end
