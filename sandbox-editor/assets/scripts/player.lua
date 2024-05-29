require "hyper"
require "input_keys"

local node = {}

function node:init()
    -- body
    self.body = PhysicsBody:new(self.owner, self.id())
    self.is_jumping = false
    self.ground_threshold = 0.01  -- Threshold for considering the entity grounded
end

function node:update(dt)
    local move_force = vec2:new(0, 0)

    if hyper.keyPressed(KEY.A) then
        move_force.x = -1.0 * dt
    elseif hyper.keyPressed(KEY.D) then
        move_force.x = 1.0 * dt
    end

    self.body:applyImpulse(move_force, self.body:getWorldCenter(), true)

    local velocity = self.body:getLinearVelocity()

    if self.is_jumping and math.abs(velocity.y) < self.ground_threshold then
        self.is_jumping = false
    end

    -- Jump when the space key is pressed and the entity is not already jumping
    if hyper.keyPressed(KEY.SPACE) and not self.is_jumping then
        local jump_impulse = vec2:new(0, 1.0)  -- Instantaneous jump impulse
        self.body:applyImpulse(jump_impulse, self.body:getWorldCenter(), true)
        self.is_jumping = true
    end
end

function node:destroy()
    -- body
end

return node
