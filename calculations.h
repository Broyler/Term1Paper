#include "triangle.h"

extern uint64_t frequency;

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

void CalculateVertices() {
	double angle, x, y;

	for (int i = 0; i < 3; ++i) {
		angle = rotation + i * (2 * M_PI / 3);
		x = cos(angle) * radius + SCREEN_WIDTH / 2;
		y = sin(angle) * radius + SCREEN_HEIGHT / 2;
		vertices[i] = (SDL_Vertex){(SDL_FPoint){x, y}, TRIANGLE_COLORS[i]};
	}
}

void AddRotation(uint64_t* lastTick) {
	uint64_t now = SDL_GetPerformanceCounter();
	double delta = (double)(now - *lastTick) / frequency;
	*lastTick = now;
    rotation += speed * delta;
}
