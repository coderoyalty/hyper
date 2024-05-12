## Editor Reference

> alpha

The editor is primarily splitted into three windows/panel.

- The Scene hierarchy
- The Game Viewport
- The Entity Properties

The Scene hierarchy panel list out the entities a scene contains. Entities can be selected, added and removed from this panel.

The Game viewport displays the rendered entities of a scene. You can select an entity from the game viewport, and also transform an entity using the scale, rotation, and translation gizmo.

The Properties panel contains tools to manipulate the selected entity components. We can set colors, add textures, rename a component etc. right in the properties panel.

### How To Navigate The Editor

Firstly, right-click the scene hierarchy panel and select the popup menu to create an entity. Select the new entity (already done by default), you should see its components rendered in the properties panel.

Nothing is rendered on the viewport for now, that's because the current entity does not have any drawable component yet. You should see a button labelled `add component`, click it and select your preferred drawable component (for the sake of this doc, choose `SpriteComponent`).

Congrats! You can play around with the entity, and also try using the controls (see below) to manipulate your entity"

### Controls

| Hot-Keys           | Uses                     | Disabled Outside Edit Mode |
| ------------------ | ------------------------ | -------------------------- |
| `Ctrl+O`           | open scene               | True                       |
| `Ctrl+N`           | new scene                | True                       |
| `Ctrl+S`           | save a scene             | True                       |
| `Ctrl + Shift + S` | save scene as            | True                       |
| `G`                | enable translation gizmo | False                      |
| `S`                | enable scaling gizmo     | False                      |
| `Q`                | disable gizmo            | False                      |
| `R`                | enable rotation gizmo    | False                      |
| `F5`               | play or stop a scene     | False                      |

[Home 🏠](./hyper.md)<br/>
[⏩ Script Reference](./script-ref.md)
