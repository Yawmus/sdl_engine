#ifndef UI_CANVAS_H
#define UI_CANVAS_H

#include <SDL_ttf.h> 
#include <SDL_gpu.h>

#include <string>
#include <unordered_map>

struct UI_Element{
	int x;
	int y;
	GPU_Image* text_texture;
	SDL_Color color;
};

class UI_Canvas {
	private:
	TTF_Font * font;
	std::unordered_map<std::string, UI_Element> labels;

	public:
	UI_Canvas();
	~UI_Canvas();
	void Render(GPU_Target*);
	void UpdateLabel(int, int, const char*, const char*, SDL_Color);
	std::unordered_map<std::string, UI_Element> GetLabels();
};

#endif
