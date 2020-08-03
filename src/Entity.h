#ifndef ENTITY_H
#define ENTITY_H

#include <SDL_gpu.h>

struct Entity {
	int x;
	int y;
	int id;
	int prop;
	int width;
	int height;
	GPU_Image* image;

	struct Transform {
		int dx;
		int dy;
	} transform;

};

#endif

