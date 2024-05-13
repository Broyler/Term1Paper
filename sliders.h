#include "calculations.h"

typedef struct {
	const int index;
	const double min;
	const double max;
	double* value;
	SDL_Rect rect;
	bool dragged;
} Slider;

const int SLIDER_PADDING = 150;
const int SLIDER_OFFSET = 50;
const int SLIDER_WIDTH = 5;
const int HANDLE_WIDTH = 16;
const SDL_Color SLIDER_COLOR = {0x16, 0x16, 0x16, 0xff};
const SDL_Color HANDLE_COLOR = {0xff, 0xff, 0xff, 0xff};
const Slider radiusSlider = {1, 10, 200, &radius};
const Slider speedSlider = {2, -2 * M_PI, 2 * M_PI, &speed};
Slider sliders[] = {radiusSlider, speedSlider};
const int slidersAmount = 2;

void RenderSliderBars(SDL_Renderer* renderer) {
	SetRenderColor(renderer, &SLIDER_COLOR);

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
	SetRenderColor(renderer, &HANDLE_COLOR);
	
	for (int i = 0; i < slidersAmount; ++i) {
		RenderHandle(renderer, &sliders[i]);
	}
}

void RenderSliders(SDL_Renderer* renderer) {
	RenderSliderBars(renderer);
	RenderSliderHandles(renderer);
}

void UpdateSliderValues(SDL_MouseMotionEvent e) {
	for (int i = 0; i < slidersAmount; ++i) {
		if (sliders[i].dragged) {
			double progress = InverseLerp(e.x, SLIDER_PADDING, SCREEN_WIDTH - SLIDER_PADDING);
			*sliders[i].value = Lerp(progress, sliders[i].min, sliders[i].max);
		}
	}
}
