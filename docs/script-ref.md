# Script Reference

> **Last Modified - 2024-05-13**

> Hyper is still a work-in-progress engine at this time. The scripting system is in its alpha stage, and changes might happen without order; however, this guide remains viable for the scripting basics of the engine.

To attach a script to an entity, create a **.lua** file

```lua
-- Player.lua
local Player = {}

return Player
```

The **script must return a table**. It is used to access other parts of the system.

There's no need to manually create a script file. You can do that from the editor by adding a script component to an existing entity; then a modal will popup, where you can input the script path e.g `assets/scripts/player.lua`. If you've created the script manually, you can select it by clicking the `...` button, this should open a file dialog where you can select the preferred script, mind you it must have the `.lua` file extension.

A script file has three primary methods:

```lua
-- called once, when the script was loaded
function Player:init()
end

-- called for every frame
function Player:update(dt)
end

-- called when the scene ends
function Player:destroy()
end
```

the `init` and `destroy` methods can be omitted, but the `update` method is required and compulsory; In other words, there should be no script file if there's no `update` method.

#### How to interact with the entity's component

Each script is attached to an entity, as a component. An entity can have many elements, and by default has an ID, tag and transform component. Presumably, these components are always existing for an entity.

Here's how we can interact with components from the script:

```lua
function Player:init()
  self.velocity = 10
  transform = self.owner:get(self.id(), Transform)
  print(transform.position)
  print(transform.scale)
  print(transform.rotation)
end

function Player:update(dt)
  transform = self.owner:get(self.id(), Transform)
  transform.position.x = transform.position.x + self.velocity * dt -- continue moving to the right
end
```

To make the script take effect, press `F5` (or `Fn`+`F5`), this should play (or stop if already running) the editor scene.

**`self.owner`**: The S in ECS. It's an interface to the scene's registry where entities and their components are managed. It provides methods such as `has`, `get`, `valid`, `remove`, `clear` and `emplace`. These methods take two parameters, an entity ID, and the component type.

**`self.id`**: The script's entity ID.

**Available Components**:

- IDComponent
- TagComponent
- Transform
- SpriteComponent
- CircleComponent
- TextComponent

> reference `/hyper/src/scene/components.hpp` to see the component structures

### How To Modify Another Entity From A Script

> Game object is the same as entity.

In a game scene, game objects interaction is utile. Hyper's game object object are fully transparent, such that they don't have a parent nor a child. But, the behavior can be achieved via the scripting component. Hyper provides some utility functions for interacting with other entity in a scene.

```lua
require "hyper"

function Player:init()
  self.time = 0.0
  self.radius = 5.0

  entity = hyper.find_entity(self.owner, "player_2")
  if self.owner:valid(entity) then
    self.partner = entity
  end
end
```

For example, the `hyper.find_entity` helps find an entity within `self.owner` (the scene registry), with the name "`player_2`". If it exists, then we store the entity id as `self.partner`. This can later be used to modify the transformation such that the entity named "`player_2`" is relative to the current script's entity. This virtually create a parent-child relationship for these entities.

```lua
-- continuation

function Player:update(dt)
  transform = self.owner:get(self.id(), Transform)
  	if transform then
    	self.time = self.time + dt

    	transform.position.x = math.cos(self.time) * self.radius
    	transform.position.y = math.sin(self.time) * self.radius
  	end

    if self.partner then
      partner_transform = self.owner:get(self.partner, Transform)
      partner_transform.position.x = transform.position.x
      partner_transform.position.y = transform.position.y
    end
end
```

### How To Create Entities From Script

Another thing we can do is create new game objects during the scripting runtime, but this requires manually destroying them immediately in the `destroy` method.

> Rule 1. When game objects are created from script, they must be destroyed before the script runs out.

```lua
require "hyper"
function Player:init()
  self.child = hyper.create_entity(self.owner, "PlayerChild")
end

function Player:destroy()
  hyper.destroy_entity(self.owner, self.child)
end
```

> using namespace hyper;

The `create_entity` function, creates an entity in the scene with a transform component. It takes the scene registry, and entity name. Mind you, the function mitigates the risks of creating entities with the same name. Here's what it does:

1. Finds an existing entity with the tagged name
2. If it exists, it'll return the entity.
3. Else, it'll create an entity with the tagged name, add the necessary components and return the entity.

`destroy_entity` on the other hand, removes the entity from the registry if it exists.

If done right, you should be enabled to select and modify the new entity from the scene hierarchy panel.

```lua
hyper {

  find_entity(owner, name) -> entity

  create_entity(owner, name) -> entity

  destroy_entity(owner, name) -> true | false

  destroy_entity(owner, id) -> true | false

}
```

[Home 🏠](./hyper.md)<br/>
[Editor Reference 🚝](./editor-ref.md)
