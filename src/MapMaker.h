#ifndef MAPMAKER_H
#define MAPMAKER_H

#include <SDL_ttf.h> 
#include <SDL_gpu.h>
#include <SDL_events.h>
#include <string>
#include <unordered_map> 
#include "Entity.h"
#include "UI_Canvas.h"
#include "Constants.h"
#include "Helper.h"
#include "Util.h"
#include "AssetManager.h"
#include "EntityManager.h"

#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>
#include <fstream>

#define ASSET_TYPE std::unordered_map<std::string, GPU_Image*>
#define ID_TYPE std::unordered_map<int, Entity_Type*>
#define GRID_TYPE std::vector<Entity*>
#define create_asset_key(f, s, x, y) ("f: " + std::string(f) + ", s: " + std::string(s) + ", x: " + std::to_string(x) + ", y: " + std::to_string(y))

class MapMaker {
    private:
		EntityMap* entities;
		std::vector<Entity*>* bEntities;
		std::vector<Entity*>* fEntities;
		AssetManager asset_manager;
		std::unordered_map<std::string, std::string> asset_map;
		GRID_TYPE *grid;

		UI_Canvas *canvas;
		bool isRunning;
		GPU_Target *screen;
		std::vector<GPU_Rect*> palatte;
		EntityManager entity_manager;
		lua_State * L;
		int map_width, map_height;
		std::string selectedTex = "grass";

    public:
		MapMaker();
		~MapMaker();
		bool IsRunning() const;
		bool Initialize(GPU_Target* screen, std::string);
		//bool lua_DefineMetaTables();
		void ProcessInput();
		void Update(int);
		void Render();
		void Destroy();
		bool LoadState(std::string);
		bool SaveState();


		//bool SaveState();
		//bool LoadState(std::string state);
};

#endif

