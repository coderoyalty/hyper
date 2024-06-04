local node = {}

function node:init()
	-- body
	
end

function node:update(dt)
	-- body
	local transform = self.owner:get(self.id(), Transform)

    -- Move the pipe to the left
    transform.position.x = transform.position.x - 5.0 * dt

    -- Recycle pipe if it goes off-screen
    if transform.position.x < -26 then
        transform.position.x = math.random(-10, 3)
        -- transform.position.y = transform.position.y - (math.random(-1, 1) * 0.1) -- Randomize vertical position
    end
end

function node:destroy()
	-- body
end

return node
