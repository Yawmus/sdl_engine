#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <SDL_gpu.h>
#include <string>
#include <unordered_map>

#include "Constants.h"
#include "Entity.h"
#include "Util.h"

extern "C"{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

class AssetManager{
	private:
		std::unordered_map<std::string, GPU_Image*> assets;
		std::unordered_map<int, Entity_Type*> id_map;
	public:
		~AssetManager();
		void LoadAssets(lua_State*);
		GPU_Image* GetAsset(std::string);
		Entity_Type* GetEntityType(int);
		int GetProp(Entity_Type*);
		GPU_Image* LoadImage(const char*);
		void lua_printstack(lua_State*);
		int GetId(std::string);
};

#endif
