#include "AssetManager.h"

AssetManager::~AssetManager(){
	for(auto const& itr : assets){
		GPU_FreeImage(itr.second);
	}
	for(auto const& itr : id_map){
		delete itr.second;
	}
}

GPU_Image* AssetManager::GetAsset(std::string asset){
	if(assets.find(asset) == assets.end())
	{
		std::cerr << "Failed to find asset: " << asset << std::endl;
		return assets["Unknown"];
	}

	return assets[asset];
}

Entity_Type* AssetManager::GetEntityType(int id){
	if(id_map.find(id) == id_map.end())
	{
		std::cerr << "Failed to find type with id: " << id << std::endl;
		return id_map[100]; // Unknown
	}

	return id_map[id];
}

void AssetManager::LoadAssets(lua_State *L){
	if(lua_Check(L, luaL_dofile(L, "../scripts/assets.lua"))){
		std::vector<int> entity_ids {
			0,
				1,
				2,
				50,
				98,
				99,
				100 // unknown
		};
		for(int id : entity_ids){
			lua_getglobal(L, "GetEntityType");
			if(lua_isfunction(L, -1)){
				lua_pushnumber(L, id);

				if(lua_Check(L, lua_pcall(L, 1, 1, 0))){
					if(lua_istable(L, -1)){
						std::string name;
						bool blocking;
						std::string asset;
						GPU_Rect* region;

						name = lua_GetTableStr(L, "name");

						lua_pushstring(L, "blocking");
						lua_gettable(L, -2);
						blocking = lua_toboolean(L, -1);
						lua_pop(L, 1);

						asset = lua_GetTableStr(L, "asset");

						lua_pushstring(L, "region");
						lua_gettable(L, -2);
						if(lua_istable(L, -1)){
							region = new GPU_Rect{
								(float)lua_GetTableNum(L, "x"),
								(float)lua_GetTableNum(L, "y"),
								(float)lua_GetTableNum(L, "width"),
								(float)lua_GetTableNum(L, "height")
							};
							lua_pop(L, 1);
						}

						Entity_Type *et = new Entity_Type{
							name,
							blocking,
							asset,
							region
						};

						id_map[id] = et;
						lua_pop(L, 1); // Remove table from stack
					}
				}
			}
		} 
		lua_getglobal(L, "assets");
		if(lua_istable(L, -1)){
			std::vector<std::string> inputs {
				"Player",
				"Monster",
				"Terrain_tilemap",
				"Obj_tilemap"
			};
			for(std::string input : inputs){
				lua_pushstring(L, input.c_str());
				lua_gettable(L, -2);
				assets[input] = LoadImage(lua_tostring(L, -1));
				lua_pop(L, 1);
			}
			lua_pop(L, 1);
		}
	}
}


int AssetManager::GetProp(Entity_Type* et){
	int prop = 0;
	if(et->blocking){
		prop += 0b0001; // Blocked
	}

	return prop;
}

