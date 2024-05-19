#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "constants.h"

typedef struct {
	const char* text;
	const int x;
	const int y;
	SDL_Color color;
	SDL_Texture* texture;
	SDL_Rect rect;
} Text;

TTF_Font* font;
Text texts[] = {
	{"Курсовая работа №1, вар. 3.23", SCREEN_WIDTH / 2, 50, (SDL_Color){0xff, 0xff, 0xff, 0xff}},
	{"Радиус", SCREEN_WIDTH / 2, SCREEN_HEIGHT - 25, (SDL_Color){0xff, 0xff, 0xff, 0xff}},
	{"Скорость вращения", SCREEN_WIDTH / 2, SCREEN_HEIGHT - 75, (SDL_Color){0xff, 0xff, 0xff, 0xff}}
};
int textsAmount = 3;

void CreateFontTextures(SDL_Renderer* renderer) {
	for (int i = 0; i < textsAmount; ++i) {
		SDL_Surface* text = TTF_RenderUTF8_Solid(font, texts[i].text, texts[i].color);
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, text);
		texts[i].texture = texture;
		int w, h;
		SDL_QueryTexture(texture, NULL, NULL, &w, &h);
		SDL_Rect rect = {texts[i].x - w / 2, texts[i].y - h / 2, w, h};
		texts[i].rect = rect;
	}
}

void RenderTexts(SDL_Renderer* renderer) {
	for (int i = 0; i < textsAmount; ++i) {
		SDL_RenderCopy(renderer, texts[i].texture, NULL, &texts[i].rect);
	}
}
