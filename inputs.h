extern bool run;

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
