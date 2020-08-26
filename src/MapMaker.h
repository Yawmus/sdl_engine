#ifndef MAPMAKER_H
#define MAPMAKER_H

#include <SDL_ttf.h> 
#include <SDL_gpu.h>
#include <SDL_events.h>
#include <string>
#include <unordered_map> 
#include "Entity.h"
#include "UI_Canvas.h"

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

class MapMaker {
    private:
		std::vector<Entity> bEntities;
		std::vector<Entity> fEntities;

		UI_Canvas *canvas;
		bool isRunning;
		GPU_Target *screen;
		lua_State * L;
		std::vector<GPU_Rect*> palatte;
		ASSET_TYPE asset_manager;
		ID_TYPE id_map;

    public:
		MapMaker();
		~MapMaker();
		bool IsRunning() const;
		void Initialize(GPU_Target* screen, std::string);
		void ProcessInput();
		void Update(int);
		void Render();
		void BlitTexture(std::vector<Entity*>);
		GPU_Image* LoadImage(const char*);
		void Destroy();
		//bool SaveState();
		//bool LoadState(std::string state);
};

#endif

