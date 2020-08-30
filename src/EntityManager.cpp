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

std::unordered_map<std::string, Entity>* EntityManager::GetAllEntitiesRef(){
	return &entities;
}


Entity* EntityManager::InitEntity(int x, int y, int id, Z_INDEX z_index){
	Entity_Type* et = asset_manager->GetEntityType(id);
	GPU_Image* image = asset_manager->GetAsset(et->asset);
	std::string key = create_key(x, y, z_index);

	if(entities.find(key) != entities.end()){
		std::cerr << "Entity already exists at location " << x << " " << y << std::endl;
		return nullptr;
	}

	Entity &e = entities[key];
	e = Entity{
		x, // x
		y, // y
		id, // id
		GetProp(et), // prop
		et->region,
		image,
		z_index,
		LUA_NOREF // Not lua-managed
	};

	if(z_index == Z_INDEX::BACKGROUND){
		bEntities.push_back(&e);
	}
	else if(z_index == Z_INDEX::FOREGROUND){
		fEntities.push_back(&e);
	}

	if(e.id == 99){ // Player
		player = &e;
	}

	return &e;
}

void EntityManager::DestroyEntity(Entity *_e){
	std::cout << "_e: " << _e << std::endl;
	std::cout << "zE size: " << bEntities.size() << std::endl;
	if(_e->z_index == Z_INDEX::BACKGROUND){
		auto it = find(bEntities.begin(), bEntities.end(), _e);
		if(it != bEntities.end()){
			int idx = 0;
			idx = distance(bEntities.begin(), it);
			bEntities.erase(bEntities.begin() + idx);
		}
		else{
			std::cout << "Not found" << std::endl;

		}
	}
	else if(_e->z_index == Z_INDEX::FOREGROUND){
		auto it = find(fEntities.begin(), fEntities.end(), _e);
		if(it != fEntities.end()){
			int idx = 0;
			idx = distance(fEntities.begin(), it);
			fEntities.erase(fEntities.begin() + idx);
		}
	}
	std::cout << "zE size: " << bEntities.size() << std::endl;
	std::cout << "_e: " << _e << std::endl;
	std::cout << "e size: " << entities.size() << std::endl;
	std::string key = create_key(_e->x, _e->y, _e->z_index);
	entities.erase(key);
	std::cout << "e size: " << entities.size() << std::endl;
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


