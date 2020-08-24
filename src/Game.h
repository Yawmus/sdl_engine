#ifndef GAME_H
#define GAME_H

#include <SDL_gpu.h>
#include <SDL_events.h>
#include <string>
#include <unordered_map> 
#include "Entity.h"

extern "C"{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#define ASSET_TYPE std::unordered_map<std::string, GPU_Image*>
#define ID_TYPE std::unordered_map<int, Entity_Type*>



class Game {
    private:
		bool isRunning;
		GPU_Target *screen;
		lua_State * L;
		std::unordered_map<std::string, char> input_handler;
		ASSET_TYPE asset_manager;
		ID_TYPE id_map;

    public:
		Game();
		~Game();
		bool IsRunning() const;
		void Initialize(int width, int height, std::string map);
		void ProcessInput();
		void Update(int);
		void Move(Entity*, Transform);
		Entity* InitEntity(int, int, int);
		void Render();
		void BlitTexture(std::vector<Entity*>);
		GPU_Image* LoadImage(const char*);
		bool LoadMap(std::string);
		void Destroy();

		std::string lua_GetTableStr(lua_State*, const char*);
		lua_Number lua_GetTableNum(lua_State*, const char*);
		bool lua_Check(lua_State*, int);
		bool lua_DefineMetaTables();
		void lua_Update(float);
		void lua_Init();
};

#endif
