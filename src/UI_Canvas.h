#ifndef UI_CANVAS_H
#define UI_CANVAS_H

#include <SDL_ttf.h> 
#include <SDL_gpu.h>

#include <string>
#include <unordered_map>
#include "Constants.h"

struct UI_Element{
	int x;
	int y;
	GPU_Image* text_texture;
	SDL_Color color;
};

class UI_Canvas {
	private:
	std::unordered_map<std::string, UI_Element> labels;
	std::unordered_map<std::string, TTF_Font*> fonts;

	public:
	UI_Canvas();
	~UI_Canvas();
	void Render(GPU_Target*);
	void UpdateLabel(int, int, const char*, const char*, SDL_Color, const char*);
	std::unordered_map<std::string, UI_Element> GetLabels();
};

#endif
