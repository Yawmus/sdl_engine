#include "MapMaker.h"


MapMaker::MapMaker() : entity_manager(&asset_manager) {
	this->isRunning = false;
	map_width = 15;
	map_height = 15;
}

void MapMaker::Initialize(GPU_Target *screen, std::string file_name)
{
	this->screen = screen;

 	L = luaL_newstate();
	luaL_openlibs(L);

	asset_manager.LoadAssets(L);

	// Init entity manager
	entity_manager.Alloc();
	fEntities = entity_manager.GetEntities(Z_INDEX::FOREGROUND);
	bEntities = entity_manager.GetEntities(Z_INDEX::BACKGROUND);

	canvas = new UI_Canvas();
	canvas->UpdateLabel(70, WINDOW_HEIGHT - 20, "Map maker", "title", C_WHITE, "md");
	canvas->UpdateLabel(210, WINDOW_HEIGHT - 18, VERSION.c_str(), "version", C_WHITE, "sm");


	grid = LoadMap(L, file_name, map_width, map_height, &entity_manager);
	if(grid == 0){
		std::cerr << "Unable to open map file" << std::endl;
		return;
	}

	isRunning = true;
	return;
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
			else if(event.key.keysym.sym == SDL_KeyCode::SDLK_1) {
				prog_state = PROGRAM_STATE::GAME;
				isRunning = false;
			}
			else if(event.key.keysym.sym == SDL_KeyCode::SDLK_2) {
			}
			else if(event.key.keysym.sym == SDL_KeyCode::SDLK_3) {
			}
			else if(event.key.keysym.sym == SDL_KeyCode::SDLK_4) {
			}
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
	GPU_Image *img = LoadImage("../assets/images/gray.png");
	GPU_Rect *region = new GPU_Rect{ 0, 0, (float)img->base_w, (float)img->base_h };
	float scaleX = map_width;
	float scaleY = map_height;
	float x = GRID_OFFSET_X + (scaleX * region->w) / 2;
	float y = GRID_OFFSET_Y + (scaleY * region->h) / 2;
	GPU_BlitTransform(img, region, screen, x, y, 0, scaleX, scaleY);


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
