#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <SDL_gpu.h>
#include <string>
#include <unordered_map>

#include "Constants.h"
#include "AssetManager.h"

#define EntityMap std::unordered_map<std::string, Entity>
#define create_key(x, y, z_index) ("x: " + std::to_string(x) + ", y: " + std::to_string(y) + ", z_index: " + std::to_string(z_index))

class EntityManager{
	private:
		AssetManager *asset_manager;
		std::vector<Entity*> fEntities;
		std::vector<Entity*> bEntities;
		EntityMap entities;
		Entity* player;
	public:
		EntityManager(AssetManager*);
		~EntityManager();
		std::vector<Entity*>* GetEntitiesRef(Z_INDEX);
		std::unordered_map<std::string, Entity>* GetAllEntitiesRef();
		void DestroyEntity(Entity *e);
		Entity* GetPlayer() const;
		Entity* InitEntity(int, int, int, Z_INDEX);

		void Alloc();
		void Clear();
};

#endif

