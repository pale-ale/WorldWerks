# WorldWerks
WorldWerks is meant to be used as a way to play pen & paper
RPGs via an internet connection, requiring neither pen nor paper.
It will also come with various automations for combat situations,
like default AI behaviours and a turn tracker. These are aimed to reduce the amount
of information that needs to be tracked manually and can be extended
via python scripting.

The state of things:
-
* UI system: manages widget creation, ownership and events
* Sprite Loader: Loads sprites from files, which can be customized
* World Map: Displays a map, allows panning and zooming, 
    and displays tokens on the map

Planned features:
-
* Level building with <https://www.mapeditor.org/>
* Loading and saving of campaigns as jsons
* Dynamic lighting and vision system
* Roles (e.g. Player, Spectator, DM)
    - Ownership
    - Vision / Visibility
    - Permissions
* Visual Customizations
    - Custom sprite usage
* Gameplay Customizations through python hooks
    - Character behaviour / AI, spells, items, skills, ...
 