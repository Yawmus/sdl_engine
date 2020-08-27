#ifndef HELPER_H
#define HELPER_H

#include <SDL_gpu.h>
#include <SDL.h>
#include <fstream>
#include <string>
#include "Entity.h"
#include "Constants.h"
#include "EntityManager.h"

extern "C"{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}


static GPU_Image* LoadImage(const char* image_path){
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

static void BlitTexture(GPU_Target *screen, std::vector<Entity*> eArr){
	for(Entity* e : eArr)
	{
		// Offset

		// Adjust to unit size
		float x = e->x * (UNIT_WIDTH_PADDED) + GRID_OFFSET_X;
		float y = e->y * (UNIT_HEIGHT_PADDED) + GRID_OFFSET_Y;

		// Scale
		float scaleW = UNIT_WIDTH / e->region->w;
		float scaleH = UNIT_HEIGHT / e->region->h;

		// Top-left pivot offsets
		x += UNIT_WIDTH / 2;
		y += UNIT_HEIGHT / 2;
		GPU_BlitTransform(e->image, e->region, screen, x, y, 0, scaleW, scaleH);
	}
}



static bool lua_Check(lua_State *L, int r){
	if(r != LUA_OK){
		std::string errmsg = lua_tostring(L, -1);
		std::cout << errmsg << std::endl;
		return false;
	}
	return true;
}

static std::string lua_GetTableStr(lua_State *L, const char* property){
	lua_pushstring(L, property);
	lua_gettable(L, -2);
	std::string ret = lua_tostring(L, -1);
	lua_pop(L, 1);

	return ret;
}

static lua_Number lua_GetTableNum(lua_State *L, const char* property){
	lua_pushstring(L, property);
	lua_gettable(L, -2);
	float ret = lua_tonumber(L, -1);
	lua_pop(L, 1);

	return ret;
}

static int GetProp(Entity_Type* et){
	int prop = 0;
	if(et->blocking){
		prop += 0b0001; // Blocked
	}

	return prop;
}



static std::vector<Entity*>* LoadMap(lua_State *L, std::string map_name, int &map_width, int &map_height, EntityManager* entity_manager){
	if(map_name == ""){
		map_name = "default";
	}
	std::cout << "Loading map: " << map_name << std::endl;
	std::ifstream map_file("../assets/maps/" + map_name + ".map");
	if (!map_file.is_open())
	{
		return 0;
	}

	std::string line;
	int x = 0, y = 0;

	// Do a quick pass for dimensions before reading data
	while (std::getline(map_file, line))
	{
		std::istringstream iss(line);
		int val;
		x = 0;
		while (iss >> val)
		{
			x++;
		}
		y++;
	}
	map_width = x;
	map_height = y;

	std::cout << map_width << std::endl;
	std::cout << map_height << std::endl;

	std::vector<Entity*> *grid = new std::vector<Entity*>[map_width * map_height];

	map_file.clear();
	map_file.seekg(0, std::ios::beg);

	y = 0;

	while (std::getline(map_file, line))
	{
		std::istringstream iss(line);
		int val;
		x = 0;
		while (iss >> val)
		{
			Entity *e = entity_manager->InitEntity(x, y, val, Z_INDEX::BACKGROUND);
			grid[y * map_height + x].push_back(e);

			std::string sVal;
			if(val < 10){
				sVal = "0" + std::to_string(val);
			}
			std::cout << sVal << " ";

			x++;
		}
		std::cout << std::endl;
		y++;
	}
	map_file.close();

	x = 1;
	y = 1;
	Entity *p = entity_manager->InitEntity(x, y, 99, Z_INDEX::FOREGROUND);
	grid[y * map_height + x].push_back(p);

	if(lua_Check(L, luaL_dofile(L, "../scripts/game.lua"))){
		lua_getglobal(L, "Main");
		if(lua_isfunction(L, -1)){
			if(lua_Check(L, lua_pcall(L, 0, 0, 0))){
			}
		}
	}

	return grid;
}

#endif
