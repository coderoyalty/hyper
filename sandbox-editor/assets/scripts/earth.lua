local node = {}

function node:init()
	-- body
	transform = self.owner:get(self.id(), Transform)
  	-- transform.position.x = 0.0
  	self.time = 0.0
  	self.radius = 5.0
  	id = self.owner:get(self.id(), IDComponent)
  	print(id.id)

  	tag = self.owner:get(self.id(), TagComponent)
  	print(tag.name)
end

function node:update(dt)
	-- body
	transform = self.owner:get(self.id(), Transform)
  	-- Check if the transform component exists
  	if transform then
    	-- Update the position based on the direction of movement
    	self.time = self.time + dt

    	transform.position.x = math.sin(self.time) * self.radius
    	transform.position.z = math.cos(self.time) * self.radius
  	end
end

function node:destroy()
	-- body
end

return node
