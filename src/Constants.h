#ifndef CONSTANT_H
#define CONSTANT_H

#include <string>
#include <SDL.h>
#include <SDL_ttf.h>

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;
const float UNIT_WIDTH = 32;
const float UNIT_HEIGHT = 32;
const float UNIT_PADDING = 0;
const float UNIT_HEIGHT_PADDED = UNIT_HEIGHT + UNIT_PADDING;
const float UNIT_WIDTH_PADDED = UNIT_WIDTH + UNIT_PADDING;

const unsigned int LUA_UPDATE_DELTA = 500;
// MAJOR.MINOR.PATCH-STAGE
extern std::string VERSION;


const SDL_Color C_WHITE = { 255, 255, 255 };
const SDL_Color C_RED = { 255, 0, 0 };
const SDL_Color C_GREEN = { 0, 255, 0 };

extern TTF_Font * font_sm;
extern TTF_Font * font_md;
extern TTF_Font * font_lg;


extern std::string fps;

enum Z_INDEX {
	BACKGROUND = 0,
	FOREGROUND = 1
};

enum PROGRAM_STATE {
	NONE = 0,
	GAME = 1,
	MAP_MAKER = 2
};

extern PROGRAM_STATE prog_state;

#endif
