#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const SDL_Color backgroundColor = {0x3b, 0x3e, 0x4f, 0xff};

bool run = true;
double speed;
double radius;

SDL_Window* InitSDL() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return NULL;

    return SDL_CreateWindow(
    	"Курсовая работа №1",
    	SDL_WINDOWPOS_CENTERED,
    	SDL_WINDOWPOS_CENTERED,
    	SCREEN_WIDTH,
    	SCREEN_HEIGHT,
    	SDL_WINDOW_SHOWN
    );
}

void EventLoop(SDL_Event* e) {
    while(SDL_PollEvent(e) != 0)
    {
    	switch (e->type) {
    	case SDL_QUIT:
    		run = false;
    		break;
    	}
	}
}

void Render(SDL_Renderer* renderer, const SDL_Color* bg) {
	SDL_SetRenderDrawColor(renderer, bg->r, bg->g, bg->b, bg->a);
	SDL_RenderClear(renderer);
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

    SDL_Event e;

    while (run) {
    	EventLoop(&e);
    	Render(renderer, &backgroundColor);
    }
    
    SDL_DestroyWindow(window);
    SDL_Quit();

	return 0;
} 
