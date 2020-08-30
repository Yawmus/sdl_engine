#include <iostream>
#include "Constants.h"
#include "Game.h"
#include "MapMaker.h"

#include <SDL_ttf.h> 

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}


std::string VERSION = "1.0.1-alpha";
std::string fps = "--";
PROGRAM_STATE prog_state = PROGRAM_STATE::GAME;


int main(int argc, char* argv[])
{
	GPU_SetDebugLevel(GPU_DEBUG_LEVEL_2);
	if (GPU_GetDebugLevel() >= GPU_DEBUG_LEVEL_0) {
		std::cout << "C++ version: " << __cplusplus << std::endl;
	}

	// Process args
	std::string map;
	if (argc >= 2)
	{
		map = argv[1];
	}

	
	GPU_Target *screen = GPU_Init(WINDOW_WIDTH, WINDOW_HEIGHT, GPU_DEFAULT_INIT_FLAGS);
	if(screen == NULL){
		std::cerr << "Error initializing SDL" << std::endl;
		return 1;
	}

	while(prog_state != PROGRAM_STATE::NONE){

		// Game
		if(prog_state == PROGRAM_STATE::GAME)
		{
			Game* game = new Game();
			game->Initialize(screen, map);

			unsigned int delta, ticks, prev_ticks, frame_count = 0, start_time;
			prev_ticks = start_time = SDL_GetTicks();

			while (game->IsRunning())
			{
				ticks = SDL_GetTicks();
				delta = ticks - prev_ticks;

				game->ProcessInput();
				game->Update(delta);
				game->Render();

				prev_ticks = ticks;
				frame_count++;
				if (frame_count % 50 == 0) {
					char* buf;
					size_t sz;
					float temp = 1000.0f * frame_count / (ticks - start_time);
					sz = snprintf(NULL, 0, "%.2f", temp);
					buf = (char*)malloc(sz + 1);
					snprintf(buf, sz + 1, "%.2f", temp);
					fps = std::string(buf);
				}
			}
			game->Destroy();
		}
		// Map Maker
		else if(prog_state == PROGRAM_STATE::MAP_MAKER)
		{
			MapMaker* map_maker = new MapMaker();
			map_maker->Initialize(screen, map);

			unsigned int delta, ticks, prev_ticks, frame_count = 0, start_time;
			prev_ticks = start_time = SDL_GetTicks();

			while (map_maker->IsRunning())
			{
				ticks = SDL_GetTicks();
				delta = ticks - prev_ticks;

				map_maker->ProcessInput();
				map_maker->Update(delta);
				map_maker->Render();

				prev_ticks = ticks;
				frame_count++;
				if (frame_count % 50 == 0) {
					char* buf;
					size_t sz;
					float temp = 1000.0f * frame_count / (ticks - start_time);
					sz = snprintf(NULL, 0, "%.2f", temp);
					buf = (char*)malloc(sz + 1);
					snprintf(buf, sz + 1, "%.2f", temp);
					fps = std::string(buf);
				}
			}
			map_maker->Destroy();
		}
	}
	GPU_Quit();

	return 0;
}
