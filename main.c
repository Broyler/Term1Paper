#define SDL_MAIN_HANDLED

#include <math.h>
#include <SDL.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
	const int index;
	const double min;
	const double max;
	double* value;
	SDL_Rect rect;
	bool dragged;
} Slider;

bool run = true;
double speed = M_PI / 2;  // Обороты в секунду
double radius = 100;
double rotation;
uint64_t frequency;
SDL_Vertex vertices[3];

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const SDL_Color BACKGROUND_COLOR = {0x3b, 0x3e, 0x4f, 0xff};
const SDL_Color TRIANGLE_COLORS[3] = {
	{0xff, 0, 0, 0xff},
	{0, 0, 0xff, 0xff},
	{0, 0xff, 0, 0xff}
};
const int SLIDER_PADDING = 150;
const int SLIDER_OFFSET = 50;
const int SLIDER_WIDTH = 5;
const int HANDLE_WIDTH = 13;
const SDL_Color SLIDER_COLOR = {0, 0, 0, 0xff};
const SDL_Color HANDLE_COLOR = {0xff, 0xff, 0xff, 0xff};
const Slider radiusSlider = {1, 10, 200, &radius};
const Slider speedSlider = {2, -2 * M_PI, 2 * M_PI, &speed};
Slider sliders[] = {radiusSlider, speedSlider};
const int slidersAmount = 2;

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

double Lerp(double x, double min, double max) {
	if (x <= 0)
		return min;

	if (x >= 1)
		return max;

	return x * (max - min) + min;
}

double InverseLerp(double x, double min, double max) {
	if (x <= min)
		return 0;

	if (x >= max)
		return 1;

	return (x - min) / (max - min);
}

void RenderSliderBars(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(
		renderer,
		SLIDER_COLOR.r,
		SLIDER_COLOR.g,
		SLIDER_COLOR.b,
		SLIDER_COLOR.a
	);

	for (int i = 1; i < 3; ++i) {
		SDL_RenderFillRect(renderer, &(SDL_Rect){
			SLIDER_PADDING,
			SCREEN_HEIGHT - SLIDER_OFFSET * i,
			SCREEN_WIDTH - 2 * SLIDER_PADDING,
			SLIDER_WIDTH
		});
	}
}

void RenderHandle(SDL_Renderer* renderer, Slider* slider) {
	double percentage = InverseLerp(*slider->value, slider->min, slider->max);
	int x = percentage * (SCREEN_WIDTH - 2 * SLIDER_PADDING) + SLIDER_PADDING;
	int y = SCREEN_HEIGHT - SLIDER_OFFSET * slider->index;
	y -= (HANDLE_WIDTH - SLIDER_WIDTH) / 2;
	slider->rect = (SDL_Rect){x, y, HANDLE_WIDTH, HANDLE_WIDTH};
	SDL_RenderFillRect(renderer, &slider->rect);
}

void RenderSliderHandles(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(
		renderer,
		HANDLE_COLOR.r,
		HANDLE_COLOR.g,
		HANDLE_COLOR.b,
		HANDLE_COLOR.a
	);
	for (int i = 0; i < slidersAmount; ++i) {
		RenderHandle(renderer, &sliders[i]);
	}
}

void RenderSliders(SDL_Renderer* renderer) {
	RenderSliderBars(renderer);
	RenderSliderHandles(renderer);
}

void CalculateVertices() {
	double angle, x, y;

	for (int i = 0; i < 3; ++i) {
		angle = rotation + i * (2 * M_PI / 3);
		x = cos(angle) * radius + SCREEN_WIDTH / 2;
		y = sin(angle) * radius + SCREEN_HEIGHT / 2;
		vertices[i] = (SDL_Vertex){(SDL_FPoint){x, y}, TRIANGLE_COLORS[i]};
	}
}

void MouseDown(SDL_MouseButtonEvent e) {
	for (int i = 0; i < slidersAmount; ++i) {
		if (SDL_PointInRect(&(SDL_Point){e.x, e.y}, &sliders[i].rect))
			sliders[i].dragged = true;
	}
}

void MouseUp(SDL_MouseButtonEvent e) {
	for (int i = 0; i < slidersAmount; ++i) {
		if (sliders[i].dragged)
			sliders[i].dragged = false;
	}
}

void UpdateSliderValues(SDL_MouseMotionEvent e) {
	for (int i = 0; i < slidersAmount; ++i) {
		if (sliders[i].dragged) {
			double progress = InverseLerp(e.x, SLIDER_PADDING, SCREEN_WIDTH - SLIDER_PADDING);
			*sliders[i].value = Lerp(progress, sliders[i].min, sliders[i].max);
		}
	}
}

void EventLoop(SDL_Event* e) {
    while(SDL_PollEvent(e) != 0)
    {
    	switch (e->type) {
    	case SDL_QUIT:
    		run = false;
    		break;

    	case SDL_MOUSEBUTTONDOWN:
    		MouseDown(e->button);
    		break;

    	case SDL_MOUSEBUTTONUP:
    		MouseUp(e->button);
    		break;

    	case SDL_MOUSEMOTION:
    		UpdateSliderValues(e->motion);
    		break;
    	}
	}
}

void Render(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(
		renderer,
		BACKGROUND_COLOR.r,
		BACKGROUND_COLOR.g,
		BACKGROUND_COLOR.b,
		BACKGROUND_COLOR.a
	);
	SDL_RenderClear(renderer);
	SDL_RenderGeometry(renderer, NULL, vertices, 3, NULL, 0);
	RenderSliders(renderer);
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
    	Render(renderer);
    }
    
    SDL_DestroyWindow(window);
    SDL_Quit();

	return 0;
} 
