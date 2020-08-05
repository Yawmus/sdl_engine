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



class Game {
    private:
		bool isRunning;
		GPU_Target *screen;
		lua_State * L;
		std::unordered_map<std::string, char> input_handler;
		std::unordered_map<std::string, GPU_Image*> asset_manager;
		std::unordered_map<int, Entity_Type*> id_map;

    public:
		Game();
		~Game();
		bool IsRunning() const;
		void Initialize(int width, int height, std::string map);
		void ProcessInput();
		void Update();
		Entity* InitEntity(int, int, int);
		void Render();
		void BlitTexture(std::vector<Entity*>);
		GPU_Image* LoadImage(const char*);
		bool LoadMap(std::string);
		void Destroy();

		std::string lua_GetTableStr(lua_State*, const char*);
		float lua_GetTableNum(lua_State*, const char*);
		bool lua_Check(lua_State*, int);
		void lua_LoadConfig();
		void lua_Update();
		void lua_LoadAssets();
};

#endif
