#define SDL_MAIN_HANDLED

#include <math.h>
#include <SDL.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const SDL_Color backgroundColor = {0x3b, 0x3e, 0x4f, 0xff};
const SDL_Color triangleColors[3] = {
	{0xff, 0, 0, 0xff},
	{0, 0, 0xff, 0xff},
	{0, 0xff, 0, 0xff}
};

bool run = true;
double speed = M_PI / 2;  // Обороты в секунду
double radius = 100;
double rotation;
uint64_t frequency;
SDL_Vertex vertices[3];

SDL_Window* InitSDL() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return NULL;

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

void CalculateVertices() {
	double angle, x, y;

	for (int i = 0; i < 3; ++i) {
		angle = rotation + i * (2 * M_PI / 3);
		x = cos(angle) * radius + SCREEN_WIDTH / 2;
		y = sin(angle) * radius + SCREEN_HEIGHT / 2;
		vertices[i] = (SDL_Vertex){(SDL_FPoint){x, y}, triangleColors[i]};
	}
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
	SDL_RenderGeometry(renderer, NULL, vertices, 3, NULL, 0);
	SDL_RenderPresent(renderer);
}

void AddRotation(uint64_t* lastTick) {
	uint64_t now = SDL_GetPerformanceCounter();
	double delta = (double)(now - *lastTick) / frequency;
	*lastTick = now;
    rotation += speed * delta;
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
    uint64_t lastTick = SDL_GetPerformanceCounter();

    while (run) {
    	EventLoop(&e);
    	CalculateVertices();
    	AddRotation(&lastTick);
    	Render(renderer, &backgroundColor);
    }
    
    SDL_DestroyWindow(window);
    SDL_Quit();

	return 0;
} 
