local Player = {}

function Player:init()
  -- Retrieve the Transform component from the owner entity
  transform = self.owner:get(self.id(), Transform)
  -- Set the initial direction of movement
  self.direction = 1 -- 1 for moving to the right, -1 for moving to the left
  -- Set the initial time
  self.time = 0
  self.bound = 10.0
end

function Player:update(dt)
  -- Retrieve the Transform component from the owner entity
  transform = self.owner:get(self.id(), Transform)
  -- Check if the transform component exists
  if transform then
    -- Update time
    self.time = self.time + dt
    -- Update the vertical position using a sine-wave pattern
    transform.position.x = math.cos(self.time) * 2
    transform.position.y = math.sin(self.time) * 2 -- Adjust the amplitude as needed
  end
end

function Player:destroy()
  -- Print a message when the player is destroyed
  print("Player destroyed")
end

return Player
