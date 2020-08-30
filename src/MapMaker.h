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
#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>
#include <fstream>

#define ASSET_TYPE std::unordered_map<std::string, GPU_Image*>
#define ID_TYPE std::unordered_map<int, Entity_Type*>
#define GRID_TYPE std::vector<Entity*>

class MapMaker {
    private:
		std::vector<Entity>* entities;
		std::vector<Entity*>* bEntities;
		std::vector<Entity*>* fEntities;
		GRID_TYPE *grid;

		UI_Canvas *canvas;
		bool isRunning;
		GPU_Target *screen;
		std::vector<GPU_Rect*> palatte;
		AssetManager asset_manager;
		EntityManager entity_manager;
		lua_State * L;
		int map_width, map_height;

    public:
		MapMaker();
		~MapMaker();
		bool IsRunning() const;
		void Initialize(GPU_Target* screen, std::string);
		//bool lua_DefineMetaTables();
		void ProcessInput();
		void Update(int);
		void Render();
		void Destroy();
		//bool SaveState();
		//bool LoadState(std::string state);
};

#endif

