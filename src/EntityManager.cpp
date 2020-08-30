#include "EntityManager.h"

EntityManager::EntityManager(AssetManager *asset_manager){
	this->asset_manager = asset_manager;
}

EntityManager::~EntityManager(){
	Clear();
}

std::vector<Entity*>* EntityManager::GetEntitiesRef(Z_INDEX z_index){
	if(z_index == Z_INDEX::FOREGROUND){
		return &fEntities;
	}
	else if(z_index == Z_INDEX::BACKGROUND){
		return &bEntities;
	}
	return nullptr;
}

std::vector<Entity>* EntityManager::GetAllEntitiesRef(){
	return &entities;
}


Entity* EntityManager::InitEntity(int x, int y, int id, Z_INDEX z_index){
	Entity_Type* et = asset_manager->GetEntityType(id);
	GPU_Image* image = asset_manager->GetAsset(et->asset);

	entities.push_back(Entity{
		x, // x
		y, // y
		id, // id
		GetProp(et), // prop
		et->region,
		image,
		z_index,
		LUA_NOREF // Not lua-managed
	});

	Entity *e = &entities.back();

	if(z_index == Z_INDEX::BACKGROUND){
		bEntities.push_back(e);
	}
	else if(z_index == Z_INDEX::FOREGROUND){
		fEntities.push_back(e);
	}

	if(e->id == 99){ // Player
		player = e;
	}

	return e;
}

Entity* EntityManager::GetPlayer() const{
	return player;
}

void EntityManager::Alloc(){
	// Alloc mem
	entities.reserve(1000);
	fEntities.reserve(50);
	bEntities.reserve(1000);
}

void EntityManager::Clear(){
	entities.clear();
	bEntities.clear();
	fEntities.clear();
	player = nullptr;
}


