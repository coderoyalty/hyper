require "input_keys"
local bird = {}

function bird:init()
	-- body
	self.body = PhysicsBody:new(self.owner, self.id())
	self.jump_count = 0
    self.max_jumps = 2
end

function bird:update(dt)
	-- body
	local velocity = self.body:getLinearVelocity()

    if math.abs(velocity.y) < 0.01 then
        self.jump_count = 0
    end

    if hyper.keyPressed(KEY.SPACE) and self.jump_count < self.max_jumps then
        local jump_force = vec2:new(0, 10)
        self.body:setLinearVelocity(vec2:new(velocity.x, 0)) -- Reset vertical velocity
        self.body:applyImpulse(jump_force, self.body:getWorldCenter(), true)
        self.jump_count = self.jump_count + 1
    end
end

function bird:destroy()
	-- body
end

return bird
