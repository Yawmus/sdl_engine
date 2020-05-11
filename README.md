# sdl_engine

------------------------------------------------------------------------------
5/10/20 - Working through architecture decisions

Grabbed sdl-gpu, a hybrid of SDL, with the ability to batch gfx calls and write
shaders. My current plan is to create a map editor first using this
technology, then split the project and write the core game logic in a separate
C++ project. Lastly, I want to integrate LUA to allow scripting/game design to
happen without the need to recompile the project.

In a perfect scenario, in a months time, I will have some core LUA integration
functions available (player, triggers, input handling(?), etc.) and a game
fully writable and independent from the core engine. Everything will be
hotreloaded from the map editor, game engine and LUA script changes. Once this
pipeline is complete, future 2D endeavours will be fluid to implement off of
this base-project.

PJ
------------------------------------------------------------------------------

