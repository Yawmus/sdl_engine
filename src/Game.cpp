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

void LoadLuaConfig();
bool CheckLua(lua_State *L, int r);

int IdToProp(int id){
	int prop = 0;

	switch(id){
		case 0:
		prop += 0b0001; // Blocked
		break;
	}
	
	return prop;
}

enum Ids {
	PLAYER,
	GRASS,
	TREE
} id;

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

	// Load images
	lua_LoadAssets();

	// If provided -- load map file
	if(!LoadMap(map_name)){
		std::cerr << "Unable to open map file" << std::endl;
		return;
	}

	// Read in Lua config scripts
	lua_LoadConfig();


	p = InitEntity(1, 1, 20);
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
				changed.insert(p);
				p->transform = { -1, 0 };
			}
			else if(event.key.keysym.sym == input_handler["Up"]) {
				changed.insert(p);
				p->transform = { 0, 1 };
			}
			else if(event.key.keysym.sym == input_handler["Down"]) {
				changed.insert(p);
				p->transform = { 0, -1 };
			}
			else if(event.key.keysym.sym == input_handler["Right"]) {
				changed.insert(p);
				p->transform = { 1, 0 };
			}
			break;
		default: 
			break;
	}
}


void Game::Update() {
	// Update entities
	//lua_Update();

	// Transform
	for(Entity* e : changed){
		e = new (e) Entity{
			e->x + e->transform.dx, // x
				e->y + e->transform.dy, // y
				e->id, // id
				e->prop, // prop
				e->width, // width
				e->height, // height
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
	std::string name = id_map[id];
	
	GPU_Image* image = asset_manager[id_map[id]];
	width = image->base_w;
	height = image->base_h;

	Entity *e = new Entity{
		x, // x
		y, // y
		0, // id
		0, // prop
		width, // width
		height, // height
		image
	};

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
		float scaleW = UNIT_WIDTH / e->width;
		float scaleH = UNIT_HEIGHT / e->height;

		// Top-left pivot offsets
		x += UNIT_WIDTH / 2;
		y += UNIT_HEIGHT / 2;


		GPU_BlitTransform(e->image, NULL, screen, x, y, 0, scaleW, scaleH);
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

void Game::lua_Update(){
	// Update
	if(CheckLua(L, luaL_dofile(L, "../scripts/entity.lua"))){
		lua_getglobal(L, "hello_world");
		if(lua_isfunction(L, -1)){
		}
	}
}

void Game::lua_LoadConfig(){
	// Loading input handler
	if(CheckLua(L, luaL_dofile(L, "../scripts/player_controller.lua"))){
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
	if(CheckLua(L, luaL_dofile(L, "../scripts/assets.lua"))){
		lua_getglobal(L, "ids");
		if(lua_istable(L, -1)){
			std::vector<int> inputs {
				20,
				0,
				1
			};
			for(int input : inputs){
				lua_pushnumber(L, input);
				lua_gettable(L, -2);
				id_map[input] = lua_tostring(L, -1);
				lua_pop(L, 1);
			}
		}
		lua_getglobal(L, "assets");
		if(lua_istable(L, -1)){
			std::vector<std::string> inputs {
				"Player",
				"Grass",
				"Tree"
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

bool Game::CheckLua(lua_State *L, int r){
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
	map_height = 0;
	int x = 0, y = 0;

	bEntities.reserve(1000);

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
			std::cout << val << " ";
			x++;
		}
		std::cout << std::endl;
		y++;
	}
	map_width = x;
	map_height = y;
	std::cout << map_width << std::endl;
	std::cout << map_height << std::endl;
	map_file.close();

	return true;
}


