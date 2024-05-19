#define SDL_MAIN_HANDLED

#include "math.h"
#include "SDL2/SDL.h"
#include "stdbool.h"
#include "stdint.h"
#include "stdio.h"
#include "texts.h"
#include "sliders.h"
#include "inputs.h"

bool run = true;
uint64_t frequency, lastTick;

SDL_Window* InitSDL() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return NULL;

	if (TTF_Init() == -1)
		return NULL;

	font = TTF_OpenFont("font.ttf", 24);
	frequency = SDL_GetPerformanceFrequency();

    return SDL_CreateWindow(
    	"Курсовая работа №1",
    	SDL_WINDOWPOS_CENTERED,
    	SDL_WINDOWPOS_CENTERED,
    	SCREEN_WIDTH,
    	SCREEN_HEIGHT,
    	SDL_WINDOW_SHOWN
    );
}

void Render(SDL_Renderer* renderer) {
	SetRenderColor(renderer, &BACKGROUND_COLOR);
	SDL_RenderClear(renderer);
	SDL_RenderGeometry(renderer, NULL, vertices, 3, NULL, 0);
	RenderTexts(renderer);
	RenderSliders(renderer);
	SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[]) {
    SDL_Window* window = InitSDL();
    SDL_Renderer* renderer = SDL_CreateRenderer(
    	window,
    	-1,
    	SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    
    if (window == NULL || renderer == NULL) {
    	printf("SDL error: %s\n", SDL_GetError());
    	return 1;
    }

    CreateFontTextures(renderer);

    SDL_Event e;
    lastTick = SDL_GetPerformanceCounter();

    while (run) {
    	EventLoop(&e);
    	CalculateVertices();
    	AddRotation(&lastTick);
    	Render(renderer);
    }
    
    SDL_DestroyWindow(window);
    SDL_Quit();

	return 0;
} 
