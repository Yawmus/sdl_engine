#ifndef CONSTANT_H
#define CONSTANT_H

#include <string>

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;
const float UNIT_WIDTH = 32;
const float UNIT_HEIGHT = 32;
const float UNIT_PADDING = 0;
const float UNIT_HEIGHT_PADDED = UNIT_HEIGHT + UNIT_PADDING;
const float UNIT_WIDTH_PADDED = UNIT_WIDTH + UNIT_PADDING;

const unsigned int LUA_UPDATE_DELTA = 500;

extern std::string fps;

#endif
