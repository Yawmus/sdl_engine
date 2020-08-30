#ifndef HELPER_H
#define HELPER_H

#include <SDL_gpu.h>
#include <SDL.h>
#include <fstream>
#include <string>
#include "Entity.h"
#include "Constants.h"
#include "EntityManager.h"
#include "Util.h"




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







static bool LoadMap(lua_State *L, std::string map_name, int &map_width, int &map_height, GRID_TYPE **grid, EntityManager* entity_manager){
	if(map_name == ""){
		map_name = "default";
	}
	std::cout << "Loading map: " << map_name << std::endl;
	std::ifstream map_file("../assets/maps/" + map_name + ".map");
	if (!map_file.is_open())
	{
		std::cerr << "Unable to open map file" << std::endl;
		return false;
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

	*grid = new std::vector<Entity*>[map_width * map_height];

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
			(*grid)[y * map_height + x].push_back(e);

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
	(*grid)[y * map_height + x].push_back(p);

	return true;
}

#endif
