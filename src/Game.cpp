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
#include <SDL_ttf.h> 

int map_width, map_height;

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

std::vector<Entity*> *grid;
std::set<Entity*> changed;
std::vector<Entity*> entities;
std::vector<Entity*> fEntities;
std::vector<Entity*> bEntities;
Entity *p;

GPU_Image* text_texture;
SDL_Surface * text_surface;
TTF_Font * font;

Game::Game() {
	this->isRunning = false;
}

Game::~Game() {
}

bool Game::IsRunning() const {
	return this->isRunning;
}

void Game::Initialize(int width, int height, std::string map_name)
{
	screen = GPU_Init(WINDOW_WIDTH, WINDOW_HEIGHT, GPU_DEFAULT_INIT_FLAGS);
	if(screen == NULL){
		std::cerr << "Error initializing SDL" << std::endl;
		return;
	}

	TTF_Init();
	font = TTF_OpenFont("../assets/fonts/arial.ttf", 20);

	if(!font) {
		std::cerr << "TTF_OpenFont: " << TTF_GetError() << std::endl;
	}


	// Init lua
 	L = luaL_newstate();
	luaL_openlibs(L);

	size_t nbytes = sizeof(Entity);
	Entity *e = (Entity *)lua_newuserdata(L, nbytes);
 

	// Load images
	lua_LoadAssets();


	// If provided -- load map file
	if(!LoadMap(map_name)){
		std::cerr << "Unable to open map file" << std::endl;
		return;
	}


	// Read in Lua config scripts
	lua_LoadConfig();


	p = InitEntity(1, 1, 99);
	fEntities.push_back(p);
	entities.push_back(p);

	isRunning = true;

	return;
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
				isRunning = false;
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
	int dx = e->x + t.dx;
	int dy = e->y + t.dy;

	// Map bounds
	if(dx < 0 || dy < 0 || dx >= map_width - 1 || dy >= map_height - 1){
		return;
	}
	
	// Bump
	std::vector temp = grid[dy * map_height + dx];
	for( std::vector<Entity*>::iterator iter = temp.begin(); iter != temp.end(); ++iter )
	{
		if( (*iter)->prop ==  0b0001) // Blocked
		{
			return;
		}
	}

	e->transform = t;
	changed.insert(e);
}


void Game::Update(float delta) {
	// Update entities
	//lua_Update();

	// Transform
	for(Entity* e : changed){
		Transform t = e->transform;

		// If moved
		if(t.dx || t.dy){
			int dx = e->x + t.dx;
			int dy = e->y + t.dy;

			// Add it to new loc
			grid[dy * map_height + dx].push_back(e); 

			// Remove from old loc
			std::vector<Entity*> *temp = &grid[e->y * map_height + e->x];
			temp->erase(std::remove(temp->begin(), temp->end(), e), temp->end());
		}

		e = new (e) Entity{
			e->x + e->transform.dx, // x
				e->y + e->transform.dy, // y
				e->id, // id
				e->prop, // prop
				e->region, // region
				e->image // image
		};
	}
	changed.clear();


	// Update UI
	text_surface = TTF_RenderText_Solid(font, fps.c_str(), { 255, 255, 255 });
	text_texture = GPU_CopyImageFromSurface(text_surface);
	SDL_FreeSurface(text_surface);
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

Entity* Game::InitEntity(int x, int y, int id){
	int width, height;
	if(id_map.find(id) == id_map.end())
	{
		std::cerr << "Failed to find id: " << id << std::endl;
		return NULL;
	}
	Entity_Type *et = id_map[id];
	GPU_Image* image = asset_manager[et->asset];

	Entity *e = new Entity{
		x, // x
		y, // y
		id, // id
		GetProp(et), // prop
		et->region,
		image
	};

	grid[y * map_height + x].push_back(e);

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
	GPU_Blit(text_texture, NULL, screen, WINDOW_WIDTH - 50, WINDOW_HEIGHT - 30);

	GPU_Flip(screen);
}

void Game::BlitTexture(std::vector<Entity*> eArr){
	for(Entity* e : eArr)
	{
		// Adjust to unit size
		float x = e->x * (UNIT_WIDTH_PADDED);
		float y = e->y * (UNIT_HEIGHT_PADDED);

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

	GPU_FreeImage(text_texture);

	for (auto e : entities)
	{
		delete e;
	}
	entities.clear();
	bEntities.clear();
	fEntities.clear();

	TTF_CloseFont(font);	

	TTF_Quit();
	GPU_Quit();
}

void Game::lua_Update(float delta){
	// Update
	if(lua_Check(L, luaL_dofile(L, "../scripts/entity.lua"))){
		lua_getglobal(L, "Update");
		if(lua_isfunction(L, -1)){
			for(Entity* e : entities){
				lua_pushlightuserdata(L, e);
				lua_pushnumber(L, delta);

				if(lua_Check(L, lua_pcall(L, 1, 1, 0))){
				}
			}
		}
	}
}

void Game::lua_LoadConfig(){
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
		}
	}
}

void Game::lua_LoadAssets(){
	if(lua_Check(L, luaL_dofile(L, "../scripts/assets.lua"))){
		std::vector<int> entity_ids {
			0,
				1,
				2,
				50,
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
						}

						Entity_Type *et = new Entity_Type{
							name,
							blocking,
							asset,
							region
						};

						id_map[id] = et;
					}
				}
			}
		}
		lua_getglobal(L, "assets");
		if(lua_istable(L, -1)){
			std::vector<std::string> inputs {
				"Player",
				"Terrain_tilemap",
				"Obj_tilemap"
			};
			for(std::string input : inputs){
				lua_pushstring(L, input.c_str());
				lua_gettable(L, -2);
				asset_manager[input] = LoadImage(lua_tostring(L, -1));
				lua_pop(L, 1);
			}
		}
	}
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

	entities.reserve(1000);
	bEntities.reserve(1000);

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
			Entity *tile;
			tile = InitEntity(x, y, val);
			bEntities.push_back(tile);
			entities.push_back(tile);

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

	return true;
}


