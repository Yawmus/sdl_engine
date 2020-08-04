#include <iostream>
#include "Constants.h"
#include "Game.h"

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}


std::string fps = "--";

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

	Game* game = new Game();
	game->Initialize(WINDOW_WIDTH, WINDOW_HEIGHT, map);

	unsigned int delta, ticks, prev_ticks, frame_count = 0, start_time;
	prev_ticks = start_time = SDL_GetTicks();

	while (game->IsRunning())
	{
		ticks = SDL_GetTicks();
		delta = ticks - prev_ticks;

		game->ProcessInput();
		game->Update();
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

	return 0;
}