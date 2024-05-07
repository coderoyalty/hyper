local Player = {}

function Player:init()
  -- Retrieve the Transform component from the owner entity
  transform = self.owner:get(self.id(), Transform)
  -- Set the initial direction of movement
  self.direction = 1 -- 1 for moving to the right, -1 for moving to the left
  -- Set the initial time
  self.time = 0
  self.bound = 2
end

function Player:update(dt)
  -- Retrieve the Transform component from the owner entity
  transform = self.owner:get(self.id(), Transform)
  -- Check if the transform component exists
  if transform then
    -- Update time
    self.time = self.time + dt
    -- Update the horizontal position based on the direction of movement
    local newX = transform.position.x + dt * 2.0 * self.direction -- Adjust the speed as needed
    -- Check if the horizontal position exceeds the bounds
    if newX > self.bound or newX < -self.bound then
      -- Invert the direction of movement
      self.direction = -self.direction
    else
      -- Update the horizontal position
      transform.position.x = newX
    end
    -- Update the vertical position using a sine-wave pattern
    -- transform.position.y = math.sin(self.time) * 2 -- Adjust the amplitude as needed
  end
end

function Player:destroy()
  -- Print a message when the player is destroyed
  print("Player destroyed")
end

return Player
