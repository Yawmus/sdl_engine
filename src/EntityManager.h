#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <SDL_gpu.h>
#include <string>
#include <unordered_map>

#include "Constants.h"
#include "AssetManager.h"


class EntityManager{
	private:
		AssetManager *asset_manager;
		std::vector<Entity*> fEntities;
		std::vector<Entity*> bEntities;
		std::vector<Entity> entities; // Destroyed since engine-managed
		Entity* player;
	public:
		EntityManager(AssetManager*);
		~EntityManager();
		std::vector<Entity*>* GetEntities(Z_INDEX);
		Entity* GetPlayer() const;
		Entity* InitEntity(int, int, int, Z_INDEX);

		void Alloc();
		void Clear();
};

#endif

