#ifndef GAME_H
#define GAME_H

#include <SDL_ttf.h> 
#include <SDL_gpu.h>
#include <SDL_events.h>
#include <string>
#include <unordered_map> 
#include "Entity.h"
#include "UI_Canvas.h"
#include "Constants.h"
#include "Helper.h"
#include "AssetManager.h"
#include "EntityManager.h"

extern "C"{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>
#include <fstream>


class Game {
    private:
		GRID_TYPE *grid; // For collision/tile information
		std::set<Entity*> changed; // For entities to be transformed

		std::vector<Entity*>* fEntities;
		std::vector<Entity*>* bEntities;
		std::vector<Entity>* entities; // Destroyed since engine-managed

		std::vector<Entity*> lua_entities; // Owned by Game class. lua-managed

		bool isRunning;
		GPU_Target *screen;
		lua_State * L;
		std::unordered_map<std::string, char> input_handler;
		EntityManager entity_manager;
		AssetManager asset_manager;
		UI_Canvas *canvas;

    public:
		Game();
		~Game();
		bool IsRunning() const;
		void Initialize(GPU_Target* screen, std::string);
		void ProcessInput();
		void Update(int);
		void Move(Entity*, Transform);
		//Entity* InitEntity(int, int, int, Z_INDEX);
		void Render();
		//void BlitTexture(std::vector<Entity*>);
		//GPU_Image* LoadImage(const char*);
		//bool LoadMap(std::string);
		void Destroy();
		bool SaveState();
		bool LoadState(std::string state);
		//void Alloc();
		//void Clear();

		//std::string lua_GetTableStr(lua_State*, const char*);
		//lua_Number lua_GetTableNum(lua_State*, const char*);
		//bool lua_Check(lua_State*, int);
		bool lua_DefineMetaTables();
		void lua_Update(float);
		void lua_Init();
		static void lua_printstack(lua_State*);
};

#endif
