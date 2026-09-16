# MasterWoW DevTools — TBC 2.4.3

Client-side developer toolkit for WoW 2.4.3 (Interface 20400). No server/core/Eluna/AIO dependency.

## Install
Copy `MasterWoWDevTools` into `Interface/AddOns/` and enable it on character select.

## Open
- `/mdev`
- `/devtools`

Quick commands:
- `/mdev run <lua>`
- `/mdev api`
- `/mdev cvar`
- `/mdev frame`
- `/mdev event`
- `/mdev item <id>`
- `/mdev spell <id>`
- `/mdev cap`

## Included modules
- Lua Console with runtime error handling and history
- Runtime API / `_G` browser
- CVar browser/editor with large known-TBC list
- Frame inspector + live mouse inspect + frame enumeration
- Event monitor with broad TBC event catalog and filtering
- Unit/target inspector
- Item inspector
- Spell inspector
- Action bar / macro / keybinding inspector
- Texture/BLP preview
- Live expression watch
- Performance / addon memory
- Client capability scanner
- Addon inspector + built-in script library

## Notes
This addon intentionally discovers functions from the running client rather than assuming a wiki is correct. That is useful with patched 2.4.3 executables.

Some game APIs are protected by normal client restrictions, require a hardware click, only work while a corresponding UI is open, or need arguments that cannot be inferred automatically. The toolkit exposes and tests them but cannot bypass those rules.
