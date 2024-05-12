### Script Reference

> Hyper is still a work-in-progress engine at this time. The scripting system is in its alpha stage, and changes might happen without order; however, this guide remains viable for the scripting basics of the engine.

To attach a script to an entity, create a **.lua**

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

[Home 🏠](./hyper.md)<br/>
[Editor Reference 🚝](./editor-ref.md)
