#ifndef ENTITY_H
#define ENTITY_H

#include <SDL_gpu.h>
#include <string>
#include <cereal/archives/json.hpp>

enum Z_INDEX {
	BACKGROUND = 0,
	FOREGROUND = 1
};

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
	Z_INDEX z_index;
	int userDataRef; // Used to have C++ ref to userdata (only lua-managed)
	Transform transform;
};

template<class Archive>
void save(Archive & archive,
               Entity const & e)
{
	archive( 
		cereal::make_nvp("x", e.x), 
		cereal::make_nvp("y", e.y), 
		cereal::make_nvp("id", e.id),
		cereal::make_nvp("z_index", e.z_index)
	);
}

template<class Archive>
void load(Archive & archive,
               Entity & e)
{
	archive( 
		e.x, 
		e.y, 
		e.id,
		e.z_index
	);
}

struct Entity_Type {
	std::string name;
	bool blocking;  // Will get condensed into prop
	std::string asset;
	GPU_Rect* region;
};

#endif

