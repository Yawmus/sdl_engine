#include "MapMaker.h"

MapMaker::MapMaker() : entity_manager(&asset_manager) {
	this->isRunning = false;
	map_width = 15;
	map_height = 15;

	asset_map[create_asset_key("grass", "flat", 0, -1)] = "grass.flat.top";
	asset_map[create_asset_key("grass", "flat", 1, -1)] = "grass.flat.topright";
	asset_map[create_asset_key("grass", "flat", 1, 0)] = "grass.flat.right";
	asset_map[create_asset_key("grass", "flat", 1, 1)] = "grass.flat.bottomright";
	asset_map[create_asset_key("grass", "flat", 0, 1)] = "grass.flat.bottom";
	asset_map[create_asset_key("grass", "flat", -1, 1)] = "grass.flat.bottomleft";
	asset_map[create_asset_key("grass", "flat", -1, 0)] = "grass.flat.left";
	asset_map[create_asset_key("grass", "flat", -1, -1)] = "grass.flat.topleft";

	//asset_map[create_asset_key("flat", "grass", 0, -1)] = "grass.flat.bottom";
	//asset_map[create_asset_key("grass", "flat", 1, -1)] = "grass.flat.bottomleft";
	//asset_map[create_asset_key("grass", "flat", 1, 0)] = "grass.flat.left";
	//asset_map[create_asset_key("grass", "flat", 1, 1)] = "grass.flat.topleft";
	//asset_map[create_asset_key("grass", "flat", 0, 1)] = "grass.flat.top";
	//asset_map[create_asset_key("grass", "flat", -1, 1)] = "grass.flat.topright";
	//asset_map[create_asset_key("grass", "flat", -1, 0)] = "grass.flat.right";
	//asset_map[create_asset_key("grass", "flat", -1, -1)] = "grass.flat.bottomright";


	asset_map[create_asset_key("water", "flat", 0, -1)] = "water.flat.top";
	asset_map[create_asset_key("water", "flat", 1, -1)] = "water.flat.topright";
	asset_map[create_asset_key("water", "flat", 1, 0)] = "water.flat.right";
	asset_map[create_asset_key("water", "flat", 1, 1)] = "water.flat.bottomright";
	asset_map[create_asset_key("water", "flat", 0, 1)] = "water.flat.bottom";
	asset_map[create_asset_key("water", "flat", -1, 1)] = "water.flat.bottomleft";
	asset_map[create_asset_key("water", "flat", -1, 0)] = "water.flat.left";
	asset_map[create_asset_key("water", "flat", -1, -1)] = "water.flat.topleft";
}



bool MapMaker::Initialize(GPU_Target *screen, std::string file_name)
{
	this->screen = screen;

 	L = luaL_newstate();
	luaL_openlibs(L);

	asset_manager.LoadAssets(L);

	// Init entity manager
	entity_manager.Alloc();
	entities = entity_manager.GetAllEntitiesRef();
	fEntities = entity_manager.GetEntitiesRef(Z_INDEX::FOREGROUND);
	bEntities = entity_manager.GetEntitiesRef(Z_INDEX::BACKGROUND);

	canvas = new UI_Canvas();
	canvas->UpdateLabel(70, WINDOW_HEIGHT - 20, "Map maker", "title", C_WHITE, "md");
	canvas->UpdateLabel(210, WINDOW_HEIGHT - 18, VERSION.c_str(), "version", C_WHITE, "sm");
	if(file_name.find(".json") != std::string::npos){
		if(!LoadState(file_name))
		{
			std::cerr << "Unable to load game" << std::endl;
			return false;
		}
	}
	else{
		if(!LoadMap(L, file_name, map_width, map_height, &entity_manager)){
			std::cerr << "Failed to load map" << std::endl;
			return false;
		}

		grid = new GRID_TYPE[map_width * map_height];
		for(Entity *e : *bEntities){
			grid[e->y * map_height + e->x].push_back(e);
		}
		for(Entity *e : *fEntities){
			grid[e->y * map_height + e->x].push_back(e);
		}
	}

	isRunning = true;
	return true;
}

bool MapMaker::SaveState(){
	{
		std::ofstream os("data.json");
		cereal::JSONOutputArchive archive(os);


		archive(cereal::make_nvp("width", map_width));
		archive(cereal::make_nvp("height", map_height));
		archive(cereal::make_nvp("entities", *entities));

		// Move lua_entities into a save-able stack format
		//std::vector<Entity> raw_lua;
		//raw_lua.reserve(lua_entities.size());
		//for(int i=0; i<lua_entities.size(); i++){
			//raw_lua.push_back(*lua_entities[i]);
		//}

		//archive(cereal::make_nvp("lua_entities", raw_lua));
	}
	return true;
}

bool MapMaker::LoadState(std::string path){
	//delete[] grid;
	entity_manager.Clear();
	//lua_entities.clear();
	
	EntityMap raw_entities; // Used to re-create entities list
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

	for (auto& it: raw_entities) {
		Entity &raw_e = it.second;
		Entity *e = entity_manager.InitEntity(raw_e.x, raw_e.y, raw_e.id, raw_e.z_index);
		grid[raw_e.y * map_height + raw_e.x].push_back(e);
	}

	raw_entities.clear();

	// Re-instantiate lua mem
	//if(lua_Check(L, luaL_dofile(L, "../scripts/game.lua"))){
		//lua_getglobal(L, "Main");
		//if(lua_isfunction(L, -1)){
			//lua_pushstring(L, path.c_str());

			//if(lua_Check(L, lua_pcall(L, 1, 0, 0))){
			//}
		//}
	//}

	return true;
}
void MapMaker::ProcessInput(){
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
			else if(event.key.keysym.sym == SDL_KeyCode::SDLK_TAB) {
				prog_state = PROGRAM_STATE::GAME;
				isRunning = false;
			}
			else if(event.key.keysym.sym == SDL_KeyCode::SDLK_1) {
				selectedTex = "grass";
			}
			else if(event.key.keysym.sym == SDL_KeyCode::SDLK_2) {
				selectedTex = "flat";
			}
			else if(event.key.keysym.sym == SDL_KeyCode::SDLK_3) {
				selectedTex = "water";
			}
			else if(event.key.keysym.sym == SDL_KeyCode::SDLK_4) {
			}
			else if(event.key.keysym.sym == SDL_KeyCode::SDLK_9) {
				SaveState();
				int x = WINDOW_WIDTH / 2 - 20;
				int y = WINDOW_HEIGHT - 30;
				canvas->UpdateLabel(x, y, "Saved", "status", C_GREEN, "md");
			}
			else if(event.key.keysym.sym == SDL_KeyCode::SDLK_0) {
				LoadState("data.json");
				int x = WINDOW_WIDTH / 2 - 20;
				int y = WINDOW_HEIGHT - 30;
				canvas->UpdateLabel(x, y, "Loaded", "status", C_WHITE, "md");
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			int x, y;
			if(SDL_BUTTON(SDL_BUTTON_LEFT) && SDL_GetMouseState(&x, &y)){
				x = (x - GRID_OFFSET_X) / UNIT_WIDTH_PADDED;
				y = (y - GRID_OFFSET_Y) / UNIT_HEIGHT_PADDED;


				std::cout << "x:" << x << ", y:" << y << std::endl;
				if(x < 0 || x >= map_width || y < 0 || y >= map_width){
					return;
				}

				// Calc tiles to replace (3x3)
				for(int i=-1; i<2; i++){
					for(int j=-1; j<2; j++){
						int dx = i + x;
						int dy = j + y;
						if(dx < 0 || dx >= map_width || dy < 0 || dy >= map_width){
							continue;
						}

						// Remove old tiles
						GRID_TYPE *vec = &grid[dy * map_height + dx];
						Entity *old_e = nullptr;
						for(Entity *e : *vec){
							if(e->z_index == Z_INDEX::BACKGROUND){
								std::cout << "in vec --\n";
								std::cout << e->x << std::endl;
								std::cout << e->y << std::endl;
								std::cout << "---\n";
								old_e = e;
							}
						}
						vec->erase(std::remove(vec->begin(), vec->end(), old_e), vec->end());
						entity_manager.DestroyEntity(old_e);

						// Place new tile
						int id = asset_manager.GetId(selectedTex);
						Entity *e = entity_manager.InitEntity(dx, dy, id, Z_INDEX::BACKGROUND);
						vec->push_back(e);
					}
				}
			}
			break;
		case SDL_MOUSEBUTTONUP:

			break;
		default: 
			break;
	}
}

void MapMaker::Update(int delta) {
}

void MapMaker::Render() {
	GPU_ClearRGB(screen, 100, 0, 120);

	// Draw grid outline (batch primitives?)
	//GPU_RectangleFilled2 (screen, rect, C_RED);


	// Draw background
	//GPU_Image *img = asset_manager.LoadImage("../assets/images/gray.png");
	//GPU_Rect *region = new GPU_Rect{ 0, 0, (float)img->base_w, (float)img->base_h };
	//float scaleX = map_width;
	//float scaleY = map_height;
	//float x = GRID_OFFSET_X + (scaleX * region->w) / 2;
	//float y = GRID_OFFSET_Y + (scaleY * region->h) / 2;
	//GPU_BlitTransform(img, region, screen, x, y, 0, scaleX, scaleY);


	// Draw game elements
	BlitTexture(screen, *bEntities);
	BlitTexture(screen, *fEntities);


	canvas->Render(screen);
	GPU_Flip(screen);
}

void MapMaker::Destroy() {
	this->screen = nullptr;
	delete canvas;
	delete[] grid;
}

bool MapMaker::IsRunning() const {
	return this->isRunning;
}

