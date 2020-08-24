#ifndef ENTITY_H
#define ENTITY_H

#include <SDL_gpu.h>
#include <string>


struct Transform {
	int dx;
	int dy;
};

struct Entity {
	int x;
	int y;
	int id;
	int prop;
	GPU_Rect* region;
	GPU_Image* image;
	Transform transform;
};

struct Entity_Type {
	std::string name;
	bool blocking;  // Will get condensed into prop
	std::string asset;
	GPU_Rect* region;
};

#endif

