#include "MapMaker.h"

MapMaker::MapMaker() {
	this->isRunning = false;
}

void MapMaker::Initialize(GPU_Target *screen, std::string file_name)
{
	this->screen = screen;

	canvas = new UI_Canvas();
	canvas->UpdateLabel(70, WINDOW_HEIGHT - 20, "Map maker", "title", C_WHITE, "md");
	canvas->UpdateLabel(210, WINDOW_HEIGHT - 18, VERSION.c_str(), "version", C_WHITE, "sm");

	//if(!LoadMap(file_name)){
		//std::cerr << "Unable to open map file" << std::endl;
		//return;
	//}

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


	canvas->Render(screen);
	GPU_Flip(screen);
}

void MapMaker::Destroy() {
	this->screen = nullptr;
	delete canvas;
}

bool MapMaker::IsRunning() const {
	return this->isRunning;
}
