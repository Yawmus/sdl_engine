#include "AssetManager.h"

AssetManager::~AssetManager(){
	for(auto const& itr : assets){
		GPU_FreeImage(itr.second);
	}
	for(auto const& itr : id_map){
		delete itr.second;
	}
}

int AssetManager::GetId(std::string name){
	for (auto& it: id_map) {
		if(it.second->name == name)
		{
			return it.first;
		}
	}
	std::cerr << "Failed to find id for name: " << name << std::endl;
	return 100;
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
		std::vector<int> entity_ids;
		lua_getglobal(L, "GetAllIds");
		if(lua_isfunction(L, -1)){
			if(lua_Check(L, lua_pcall(L, 0, 1, 0))){
				if(lua_istable(L, -1)){
					// First element is length
					int i=1;
					lua_pushinteger(L, i++);
					lua_gettable(L, -2);
					int len = (int)lua_tonumber(L, -1);
					lua_pop(L, 1);
					while(i < len+2 /* 1-indexed and offset from len */){
						lua_pushinteger(L, i++);
						lua_gettable(L, -2);
						int t = (int)lua_tonumber(L, -1);
						entity_ids.push_back(t);
						lua_pop(L, 1);
					}
					lua_pop(L, 1); // Remove table from stack
				}
			}
		}
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
void AssetManager::lua_printstack(lua_State* L)
{
    int stack = lua_gettop(L);
    for (int i = 1; i <= stack; i++)
    {
        std::cout << std::dec << i << ": " << lua_typename(L, lua_type(L, i));
        switch(lua_type(L, i))
        {
        case LUA_TBOOLEAN: std::cout << " " << lua_toboolean(L, i); break;
        case LUA_TSTRING: std::cout << " " << lua_tostring(L, i); break;
        case LUA_TNUMBER: std::cout << " " << std::dec << (uintptr_t)lua_tointeger(L, i) << " (0x" << std::hex << lua_tointeger(L, i) << ")"; break;
        default: std::cout << " " << std::hex << lua_topointer(L, i); break;
        }
        std::cout << std::endl;
    }
}
int AssetManager::GetProp(Entity_Type* et){
	int prop = 0;
	if(et->blocking){
		prop += 0b0001; // Blocked
	}

	return prop;
}

GPU_Image* AssetManager::LoadImage(const char* image_path){
	GPU_Image* image = GPU_LoadImage(image_path);
	if(image == NULL)
	{
		std::cerr << "Failed to load image: " << image_path << std::endl;
	}
	else{
		image->use_blending = false; // Opaque
	}
	return image;
}

