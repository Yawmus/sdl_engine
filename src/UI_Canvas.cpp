#include "UI_Canvas.h" 
#include <iostream>

SDL_Surface * text_surface;

UI_Canvas::UI_Canvas(){
	TTF_Init();
	font = TTF_OpenFont("../assets/fonts/arial.ttf", 20);

	if(!font) {
		std::cerr << "TTF_OpenFont: " << TTF_GetError() << std::endl;
	}
}

UI_Canvas::~UI_Canvas(){
	for(auto const& itr : labels){
		GPU_FreeImage(itr.second.text_texture);
	}

	TTF_CloseFont(font);	
	TTF_Quit();
}

void UI_Canvas::Render(GPU_Target* screen){
	for(auto const& itr : labels){
		UI_Element val = itr.second;
		GPU_Blit(val.text_texture, NULL, screen, val.x, val.y);
	}
}

std::unordered_map<std::string, UI_Element> UI_Canvas::GetLabels(){
	return labels;
}

void UI_Canvas::UpdateLabel(int x, int y, const char* text, const char* key, SDL_Color color){
	text_surface = TTF_RenderText_Solid(font, text, color);
	UI_Element ui_e = {
		x,
		y,
		GPU_CopyImageFromSurface(text_surface),
		color
	};
	SDL_FreeSurface(text_surface);
	labels[key] = ui_e;
}
