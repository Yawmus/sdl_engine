#include <iostream>
#include "Constants.h"
#include "Game.h"
#include "Entity.h"
#include <SDL.h>
#include <set>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

int map_width, map_height;
static int numLuaEntities = 0;

int GetProp(Entity_Type* et){
	int prop = 0;
	if(et->blocking){
		prop += 0b0001; // Blocked
	}

	return prop;
}


//enum Ids {
	//PLAYER,
	//GRASS,
	//TREE
//} id;


Entity *p;

void Game::Alloc(){
	// Alloc mem
	entities.reserve(1000);
	lua_entities.reserve(50);
	fEntities.reserve(50);
	bEntities.reserve(1000);
}

void Game::Clear(){
	// Destroy existing
	entities.clear();
	bEntities.clear();
	fEntities.clear();
	lua_entities.clear();

	delete[] grid;
	canvas->GetLabels().clear();
}

Game::Game() {
	this->isRunning = false;
}

Game::~Game() {
	this->screen = nullptr;
}

bool Game::IsRunning() const {
	return this->isRunning;
}

void Game::Initialize(GPU_Target *screen, std::string file_name)
{
	this->screen = screen;


	canvas = new UI_Canvas();

	// Init lua, load assets and load config scripts
	lua_Init();

	Alloc();


	if(!lua_DefineMetaTables()){
		std::cerr << "Unable to define meta tables" << std::endl;
		return;
	}

	// If provided -- load state
	if(file_name.find(".json") != std::string::npos){
		if(!LoadState(file_name))
		{
			std::cerr << "Unable to load game" << std::endl;
			return;
		}
	}
	// If provided -- load map file
	else{
		if(!LoadMap(file_name)){
			std::cerr << "Unable to open map file" << std::endl;
			return;
		}
	}



	isRunning = true;

	return;
}


bool Game::SaveState(){
	{
		std::ofstream os("data.json");
		cereal::JSONOutputArchive archive(os);


		archive(cereal::make_nvp("width", map_width));
		archive(cereal::make_nvp("height", map_height));
		archive(cereal::make_nvp("entities", entities));

		// Move lua_entities into a save-able stack format
		std::vector<Entity> raw_lua;
		raw_lua.reserve(lua_entities.size());
		for(int i=0; i<lua_entities.size(); i++){
			raw_lua.push_back(*lua_entities[i]);
		}

		archive(cereal::make_nvp("lua_entities", raw_lua));
	}
	return true;
}

bool Game::LoadState(std::string path){
	Clear();
	
	std::vector<Entity> raw_entities; // Used to re-create entities list
	// using InitEntity
	raw_entities.reserve(1000);

	{
		std::ifstream is(path);
		cereal::JSONInputArchive archive(is);

		archive(map_width);
		archive(map_height);
		archive(raw_entities);
	}

	grid = new std::vector<Entity*>[map_width * map_height];

	Alloc();

	for(int i=0; i<raw_entities.size(); i++){
		Entity raw_e = raw_entities[i];
		Entity *e = InitEntity(raw_e.x, raw_e.y, raw_e.id, raw_e.z_index);

		if(e->id == 99) /* Player */
		{
			p = e;
		}
	}

	raw_entities.clear();

	// Re-instantiate lua mem
	if(lua_Check(L, luaL_dofile(L, "../scripts/game.lua"))){
		lua_getglobal(L, "Main");
		if(lua_isfunction(L, -1)){
			lua_pushstring(L, path.c_str());

			if(lua_Check(L, lua_pcall(L, 1, 0, 0))){
			}
		}
	}

	return true;
}



void Game::lua_Init(){
	// Init lua
 	L = luaL_newstate();
	luaL_openlibs(L);

	if(lua_Check(L, luaL_dofile(L, "../scripts/assets.lua"))){
		std::vector<int> entity_ids {
			0,
				1,
				2,
				50,
				98,
				99
		};
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
		//std::cout << "Start\n";
		//lua_printstack(L);
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
				asset_manager[input] = LoadImage(lua_tostring(L, -1));
				lua_pop(L, 1);
			}
			lua_pop(L, 1);
		}
	}

	// Loading input handler
	if(lua_Check(L, luaL_dofile(L, "../scripts/player_controller.lua"))){
		lua_getglobal(L, "input_handler");
		if(lua_istable(L, -1)){
			std::vector<std::string> inputs {
				"Up",
				"Down",
				"Left",
				"Right"
			};
			for(std::string input : inputs){
				lua_pushstring(L, input.c_str());
				lua_gettable(L, -2);
				input_handler[input] = *lua_tostring(L, -1);
				lua_pop(L, 1);
			}
			lua_pop(L, 1);
		}
	}
}

void Game::ProcessInput(){
	SDL_Event event;
	SDL_PollEvent(&event);
	switch(event.type)
	{
		case SDL_QUIT:
			isRunning = false;
			break;
		case SDL_KEYDOWN: 
			//printf( "Key press detected\n" );
			if(event.key.keysym.sym == SDLK_ESCAPE) {
				prog_state = PROGRAM_STATE::NONE;
				isRunning = false;
			}
			else if(event.key.keysym.sym == SDL_KeyCode::SDLK_1) {
			}
			else if(event.key.keysym.sym == SDL_KeyCode::SDLK_2) {
				prog_state = PROGRAM_STATE::MAP_MAKER;
				isRunning = false;
			}
			else if(event.key.keysym.sym == SDL_KeyCode::SDLK_3) {
				SaveState();
				int x = 50;
				int y = WINDOW_HEIGHT - 30;
				canvas->UpdateLabel(x, y, "Saved", "status", C_GREEN);
			}
			else if(event.key.keysym.sym == SDL_KeyCode::SDLK_4) {
				LoadState("data.json");
				int x = 50;
				int y = WINDOW_HEIGHT - 30;
				canvas->UpdateLabel(x, y, "Loaded", "status", C_WHITE);
			}
			else if(event.key.keysym.sym == input_handler["Left"]) {
				Move(p, { -1, 0 });
			}
			else if(event.key.keysym.sym == input_handler["Up"]) {
				Move(p, { 0, 1 });
			}
			else if(event.key.keysym.sym == input_handler["Down"]) {
				Move(p, { 0, -1 });
			}
			else if(event.key.keysym.sym == input_handler["Right"]) {
				Move(p, { 1, 0 });
			}
			break;
		default: 
			break;
	}
}

void Game::Move(Entity *e, Transform t){
	e->transform = t;
	changed.insert(e);
}

int lua_update_tick = 0;
void Game::Update(int delta) {
	// Update entities
	
	lua_update_tick += delta;
	if(lua_update_tick >= LUA_UPDATE_DELTA) // TODO: Figure out ticks per lua_Update
	{
		lua_Update(lua_update_tick);
		lua_update_tick = 0;
	}

	// Transform
	for(Entity* e : changed){
		Transform t = e->transform;

		// If moved
		if(t.dx || t.dy){
			int dx = e->x + t.dx;
			int dy = e->y + t.dy;

			// Map bounds
			if(dx < 0 || dy < 0 || dx >= map_width - 1 || dy >= map_height - 1){
				continue;
			}

			// Bump
			bool blocked = false;
			std::vector temp = grid[dy * map_height + dx];
			for( std::vector<Entity*>::iterator iter = temp.begin(); iter != temp.end(); ++iter )
			{
				if( (*iter)->prop ==  0b0001) // Blocked
				{
					blocked = true;
				}
			}
			if(blocked){
				continue;
			}

			// Add it to new loc
			grid[dy * map_height + dx].push_back(e); 

			// Remove from old loc
			std::vector<Entity*> *temp2 = &grid[e->y * map_height + e->x];
			temp2->erase(std::remove(temp2->begin(), temp2->end(), e), temp2->end());
		}

		e = new (e) Entity{
			e->x + e->transform.dx, // x
				e->y + e->transform.dy, // y
				e->id, // id
				e->prop, // prop
				e->region, // region
				e->image, // image
				e->z_index, // z_index
				e->userDataRef // userDataRef
		};
	}
	changed.clear();

	// Update UI
	int x = WINDOW_WIDTH - 50;
	int y = WINDOW_HEIGHT - 30;
	canvas->UpdateLabel(x, y, fps.c_str(), "fps", C_WHITE);
}

GPU_Image* Game::LoadImage(const char* image_path){
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

Entity* Game::InitEntity(int x, int y, int id, Z_INDEX z_index){
	if(id_map.find(id) == id_map.end())
	{
		std::cerr << "Failed to find id: " << id << std::endl;
		return NULL;
	}
	Entity_Type *et = id_map[id];
	GPU_Image* image = asset_manager[et->asset];

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

	
	grid[y * map_height + x].push_back(&entities.back());

	return e;
}


void Game::Render() {
	// Erase
	//GPU_Clear(screen);
	GPU_ClearRGB(screen, 200, 100, 0);

	// Draw game elements
	BlitTexture(bEntities);
	BlitTexture(fEntities);

	// Draw UI
	canvas->Render(screen);

	GPU_Flip(screen);
}

void Game::BlitTexture(std::vector<Entity*> eArr){
	GPU_Rect rect = {
		5, 5, 5, 5
	};
	GPU_Rectangle2 (screen, rect, C_WHITE);
	for(Entity* e : eArr)
	{
		// Offset
		float offX = 60;
		float offY = 80;

		// Adjust to unit size
		float x = e->x * (UNIT_WIDTH_PADDED) + offX;
		float y = e->y * (UNIT_HEIGHT_PADDED) + offY;

		// Scale
		float scaleW = UNIT_WIDTH / e->region->w;
		float scaleH = UNIT_HEIGHT / e->region->h;

		// Top-left pivot offsets
		x += UNIT_WIDTH / 2;
		y += UNIT_HEIGHT / 2;
		GPU_BlitTransform(e->image, e->region, screen, x, y, 0, scaleW, scaleH);
	}
}

void Game::Destroy() {
	for(auto const& itr : asset_manager){
		GPU_FreeImage(itr.second);
	}

	Clear();
	delete canvas;
}

void Game::lua_Update(float delta){
	// Update
	lua_getglobal(L, "Entity");

	for(Entity* e : lua_entities){
		lua_pushstring(L, "Update"); 
		lua_rawget(L, -2);
		if(e->userDataRef == LUA_NOREF){
			std::cerr << "[C++] lua registry ref to userdata not set" << std::endl;
			continue;
		}
        lua_rawgeti(L, LUA_REGISTRYINDEX, e->userDataRef);
		lua_pushnumber(L, delta);
		if(lua_Check(L, lua_pcall(L, 2, 0, 0))){
		}
	}
	lua_pop(L, 1);
}


bool Game::lua_DefineMetaTables(){
	// Register cpp funcs
	auto EntityIndex = [](lua_State* L) -> int
	{
		// First arg is meta info passed, second is userdata
		const char* index = lua_tostring(L, -1);
		Entity* e = (Entity*) lua_touserdata(L, -2);

		if(strcmp(index, "x") == 0){
			lua_pushnumber(L, e->x);
		}
		else if(strcmp(index, "y") == 0){
			lua_pushnumber(L, e->y);
		}
		else{
			lua_getuservalue(L, 1); // see if they are looking to read a custom value
			lua_pushvalue(L, 2);
			lua_gettable(L, -2);
			if(lua_isnil(L, -1))
			{
				lua_getglobal(L, "Entity"); // Else get method from Entity table
				lua_pushstring(L, index);
				lua_rawget(L, -2);
				if(lua_isnil(L, -1))
				{
					std::cerr << "[C++] " << index << " does not exist" << std::endl;
				}
			}
		}
		return 1;
	};

	auto NewEntityIndex = [](lua_State* L) -> int
	{
		// First arg is meta info passed, second is userdata
		Entity* e = (Entity*) lua_touserdata(L, -3); // 1
		const char* index = lua_tostring(L, -2);     // 2
		// -1 value is the one to set                   3

		if(strcmp(index, "x") == 0){
			e->x = (int)lua_tonumber(L, -1);
		}
		else if(strcmp(index, "y") == 0){
			e->y = (int)lua_tonumber(L, -1);
		}
		else{
			lua_getuservalue(L, 1); // Get userdatum userdata table
			lua_pushvalue(L, 2);
			lua_pushvalue(L, 3);
			lua_settable(L, -3); // 1[2] = 3
		}
		return 0;
	};

	auto MoveEntity = [](lua_State* L) -> int
	{
		std::set<Entity*> *changed = (std::set<Entity*>*)lua_touserdata(L, lua_upvalueindex(1));
		Entity* e = (Entity*) lua_touserdata(L, -3);

		Transform t = {
			(int)lua_tonumber(L, -2),
			(int)lua_tonumber(L, -1)
		};
		
		e->transform = t;

		changed->insert(e);

		return 0;
	};


	auto DestroyEntity = [](lua_State* L) -> int
	{
		std::vector<Entity*> *lua_entities = (std::vector<Entity*>*)lua_touserdata(L, lua_upvalueindex(1));
		std::vector<Entity*> *fEntities = (std::vector<Entity*>*)lua_touserdata(L, lua_upvalueindex(2));
		GRID_TYPE *grid = *(GRID_TYPE**)lua_touserdata(L, lua_upvalueindex(3));
		//std::vector<Entity*> *lua_entities = (std::vector<Entity*>*)lua_touserdata(L, lua_upvalueindex(2));
		Entity* e = (Entity*) lua_touserdata(L, -1);

		// Remove from lua_entities collection
		auto it = find(lua_entities->begin(), lua_entities->end(), e);
		if(it != lua_entities->end()){
			int idx = 0;
			idx = distance(lua_entities->begin(), it);
			lua_entities->erase(lua_entities->begin() + idx);
		}

		// Remove from fEntities collection
		it = find(fEntities->begin(), fEntities->end(), e);
		if(it != fEntities->end()){
			int idx = 0;
			idx = distance(fEntities->begin(), it);
			fEntities->erase(fEntities->begin() + idx);
		}

		// Remove from grid
		std::vector<Entity*> *temp = &grid[e->y * map_height + e->x];
		temp->erase(std::remove(temp->begin(), temp->end(), e), temp->end());


        luaL_unref(L, LUA_REGISTRYINDEX, e->userDataRef);
		e->~Entity(); // Won't do anything unless I decide to create a destructor

		return 0;
	};

	auto lua_InitEntity = [](lua_State* L) -> int
	{
		std::vector<Entity*> *lua_entities = (std::vector<Entity*>*)lua_touserdata(L, lua_upvalueindex(1));
		std::vector<Entity*> *fEntities = (std::vector<Entity*>*)lua_touserdata(L, lua_upvalueindex(2));
		GRID_TYPE *grid = *(GRID_TYPE**)lua_touserdata(L, lua_upvalueindex(3));
		ASSET_TYPE* asset_manager = (ASSET_TYPE*)lua_touserdata(L, lua_upvalueindex(4));
		ID_TYPE* id_map = (ID_TYPE*)lua_touserdata(L, lua_upvalueindex(5));

		int x = (int)lua_tonumber(L, -3);
		int y = (int)lua_tonumber(L, -2);
		int id = (int)lua_tonumber(L, -1); // id

		std::cout << "[C++] Called lua_InitEntity => "; 
		std::cout << "x:" << x << ", y:" << y << ", id:" << id << std::endl;

		// Ensure it's within bounds
		if(x < 0 || y < 0 || x >= map_width - 1 || y >= map_height - 1){
			luaL_error(L, "Failed to create -- out of map bounds");
			return 1;
		}


		// void* entity = lua_newuserdata(L, sizeof(Entity));
		// new (entity) Entity(); // If I wanted to call constructor

		// Casting void* to Entity*
		// Allocating in lua and attaching metatable
		Entity* e = (Entity*)lua_newuserdata(L, sizeof(Entity));
		int userdataIdx = lua_gettop(L);

		luaL_getmetatable(L, "EntityMetaTable");
		assert(lua_istable(L, -1));
		lua_setmetatable(L, -2);

		lua_newtable(L);
		lua_setuservalue(L, 4); // Table for custom lua user values
		// Set on 4 - newuserdata

		int userDataRef = LUA_NOREF; // Used to start C++ ref to userdata

		if(id_map->find(id) == id_map->end())
		{
			std::cerr << "[C++] Failed to find id: " << id << std::endl;
		}
		Entity_Type *et = (*id_map)[id];
		GPU_Image* image = (*asset_manager)[et->asset];

		e->x = x;
		e->y = y;
		e->id = id;
		e->prop = GetProp(et);
		e->region = et->region;
		e->image = image;
		e->z_index = Z_INDEX::FOREGROUND;

		grid[y * map_height + x].push_back(e);
		fEntities->push_back(e);
		lua_entities->push_back(e);

		// Get and call userdata metatable Start method
		lua_getglobal(L, "Entity"); // Else get method from Entity table
		lua_pushstring(L, "Start"); // Invariant Start exists
		lua_rawget(L, -2);
		lua_pushvalue(L, userdataIdx); // "self"
		lua_pcall(L, 1, 0, 0);
		lua_pop(L, 1);
		
		// Store lua userdata ref
		lua_pushvalue(L, userdataIdx); // Dupe userdata
		e->userDataRef = luaL_ref(L, LUA_REGISTRYINDEX);

		return 1;
	};

	// Creating a entity table to be used with the entitymetamethod __index
	//lua_newtable(L);
	//int entityTableIdx = lua_gettop(L);
	//lua_pushvalue(L, entityTableIdx); // Puts same table on stack
	//lua_setglobal(L, "Entity"); // Name table sprite

	if(lua_Check(L, luaL_dofile(L, "../scripts/entity.lua"))){
		lua_getglobal(L, "Entity");
		if(!lua_istable(L, -1)){
			std::cerr << "[C++] No global type Entity in entity.lua" << std::endl;
			return false;
		}
	}

	// Creating a entity table to be used with the entitymetamethod __index
	//if(lua_Check(L, luaL_dofile(L, "../scripts/entity.lua"))){
		//lua_getglobal(L, "Start");
		//if(!lua_isfunction(L, -1)){
			//std::cerr << "[C++] No Start method defined in entity.lua" << std::endl;
			//return false;
		//}
		//// Attach func to global obj table
		//lua_setfield(L, -2, "Start");
	//}

	lua_pushlightuserdata(L, &lua_entities);
	lua_pushlightuserdata(L, &fEntities);
	lua_pushlightuserdata(L, &grid);
	lua_pushlightuserdata(L, &asset_manager);
	lua_pushlightuserdata(L, &id_map);
	lua_pushcclosure(L, lua_InitEntity, 5);
	lua_setfield(L, -2, "Create");

	lua_pushlightuserdata(L, &changed);
	lua_pushcclosure(L, MoveEntity, 1);
	lua_setfield(L, -2, "Move");

	//if(lua_Check(L, luaL_dofile(L, "../scripts/entity.lua"))){
		//lua_getglobal(L, "Update");
		//if(lua_isfunction(L, -1)){
		//}
	//}
	//lua_setfield(L, -2, "Update");
	//lua_pushcfunction(L, UpdateEntity);
	//lua_setfield(L, -2, "Update");


	// Define entity meta-table
	luaL_newmetatable(L, "EntityMetaTable");
	lua_pushstring(L, "__gc"); // Defining the __gc metamethod (invoked by :)
	lua_pushlightuserdata(L, &lua_entities);
	lua_pushlightuserdata(L, &fEntities);
	lua_pushlightuserdata(L, &grid);
	lua_pushcclosure(L, DestroyEntity, 3);
	lua_settable(L, -3);
	lua_pushstring(L, "__index"); // Defining the __index metamethod (invoked by :)
	lua_pushcfunction(L, EntityIndex);
	//lua_pushvalue(L, entityTableIdx); // Copying entity table to top of stack
	lua_settable(L, -3);

	lua_pushstring(L, "__newindex"); // Defining the __index metamethod (invoked by :)
	lua_pushcfunction(L, NewEntityIndex);
	lua_settable(L, -3);

	// Clean Entity and EntityMetaTable from stack
	lua_pop(L, 2);

	return true;
}

std::string Game::lua_GetTableStr(lua_State *L, const char* property){
	lua_pushstring(L, property);
	lua_gettable(L, -2);
	std::string ret = lua_tostring(L, -1);
	lua_pop(L, 1);

	return ret;
}

lua_Number Game::lua_GetTableNum(lua_State *L, const char* property){
	lua_pushstring(L, property);
	lua_gettable(L, -2);
	float ret = lua_tonumber(L, -1);
	lua_pop(L, 1);

	return ret;
}

bool Game::lua_Check(lua_State *L, int r){
	if(r != LUA_OK){
		std::string errmsg = lua_tostring(L, -1);
		std::cout << errmsg << std::endl;
		return false;
	}
	return true;
}

bool Game::LoadMap(std::string map_name){
	if(map_name == ""){
		map_name = "default";
	}
	std::cout << "Loading map: " << map_name << std::endl;
	std::ifstream map_file("../assets/maps/" + map_name + ".map");
	if (!map_file.is_open())
	{
		return false;
	}

	std::string line;
	int x = 1, y = 1;

	// Do a quick pass for dimensions before reading data
	while (std::getline(map_file, line))
	{
		std::istringstream iss(line);
		int val;
		x = 1;
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

	grid = new std::vector<Entity*>[map_width * map_height];

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
			InitEntity(x, y, val, Z_INDEX::BACKGROUND);

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
	p = InitEntity(1, 1, 99, Z_INDEX::FOREGROUND);

	if(lua_Check(L, luaL_dofile(L, "../scripts/game.lua"))){
		lua_getglobal(L, "Main");
		if(lua_isfunction(L, -1)){
			if(lua_Check(L, lua_pcall(L, 0, 0, 0))){
			}
		}
	}

	return true;
}

void Game::lua_printstack(lua_State* L)
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

