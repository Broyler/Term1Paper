double speed = M_PI / 2;  // Радианы в секунду
double radius = 100;
double rotation;
SDL_Vertex vertices[3];

void SetRenderColor(SDL_Renderer* renderer, const SDL_Color* color) {
	SDL_SetRenderDrawColor(
		renderer,
		color->r,
		color->g,
		color->b,
		color->a
	);
}
