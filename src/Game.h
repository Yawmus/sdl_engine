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

#define ASSET_TYPE std::unordered_map<std::string, GPU_Image*>
#define ID_TYPE std::unordered_map<int, Entity_Type*>
#define GRID_TYPE std::vector<Entity*>

class Game {
    private:
		std::vector<Entity*> fEntities; // For drawing foreground
		std::vector<Entity*> bEntities; // For drawing background
		GRID_TYPE *grid; // For collision/tile information
		std::set<Entity*> changed; // For entities to be transformed
		std::vector<Entity*> lua_entities; // Not destroyed since lua-managed
		std::vector<Entity> entities; // Destroyed since engine-managed

		bool isRunning;
		GPU_Target *screen;
		lua_State * L;
		std::unordered_map<std::string, char> input_handler;
		std::vector<GPU_Rect*> palatte;
		ASSET_TYPE asset_manager;
		ID_TYPE id_map;
		UI_Canvas *canvas;

    public:
		Game();
		~Game();
		bool IsRunning() const;
		void Initialize(GPU_Target* screen, std::string);
		void ProcessInput();
		void Update(int);
		void Move(Entity*, Transform);
		Entity* InitEntity(int, int, int, Z_INDEX);
		void Render();
		void BlitTexture(std::vector<Entity*>);
		GPU_Image* LoadImage(const char*);
		bool LoadMap(std::string);
		void Destroy();
		bool SaveState();
		bool LoadState(std::string state);
		void Alloc();
		void Clear();

		std::string lua_GetTableStr(lua_State*, const char*);
		lua_Number lua_GetTableNum(lua_State*, const char*);
		bool lua_Check(lua_State*, int);
		bool lua_DefineMetaTables();
		void lua_Update(float);
		void lua_Init();
		static void lua_printstack(lua_State*);
};

#endif
