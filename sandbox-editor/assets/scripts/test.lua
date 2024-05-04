-- CODE OF CONDUCT
-- must match the file name, be a global variable,
-- be unique (at least for entities)

Player = {
}

local pos = vec3:new()
local size = vec3:new()
local color = vec4:new()

size.x = 1.0
size.y = 1.0
size.z = 0.0

color.x = 1.0
color.y = 0.0
color.z = 0.0
color.w = 1.0

function Player.onCreate()
  print(pos)
  print(size)
  print(color)
end


function Player.onDestroy()
  print("onDestroy called!")
end

local time = 0.0
local fps = 0

function Player.onUpdate(dt)
  fps = fps + 1
  time = time + dt

  if time >= 1.0 then
    fps = 0
    time = 0.0
  end
end

