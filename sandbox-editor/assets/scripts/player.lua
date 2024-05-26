require "hyper"
require "input_keys"

local node = {}

function node:init()
	-- body
	self.body = PhysicsBody:new(self.owner, self.id())

	self.is_jumping = true
end

function node:update(dt)
	-- body
	transform = self.owner:get(self.id(), Transform)

	if hyper.keyPressed(KEY.A) then
		transform.position.x = transform.position.x - 2.0 * dt
	elseif hyper.keyPressed(KEY.D) then
		transform.position.x = transform.position.x + 2.0 * dt
	end

	if hyper.keyPressed(KEY.W) then
		transform.position.y = transform.position.y + 2.0 * dt
	elseif hyper.keyPressed(KEY.S) then
		transform.position.y = transform.position.y - 2.0 * dt
	end

	strength = vec2:new()
	strength.y =  0.5

	floor = hyper.find_entity(self.owner, "upper-floor")
	floor_pos = self.owner:get(floor, Transform).position


	if not self.is_jumping and hyper.keyPressed(KEY.SPACE) then
		self.body:applyImpulseToCenter(strength, true)

		self.is_jumping = true		
	end

	abs_pos = transform.position.y - transform.scale.y

	if abs_pos <= floor_pos.y or abs_pos <= 0.0 then
		self.is_jumping = false
	end

end

function node:destroy()
	-- body
end

return node
