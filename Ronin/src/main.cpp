#include <string>
#include <iostream>
#include <SDL2/SDL.h>
#ifdef __APPLE__
	#include <SDL_ttf.h>
#elif __linux__
	#include <SDL2/SDL_ttf.h>
#endif
#include "log.h"
#include "res_path.h"
#include "cleanup.h"

int main()
{
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 460;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		logSDLError(std::cout, "SDL_INIT");
		return 1;
	}

	if(TTF_Init() !=0)
	{
		logSDLError(std::cout, "TTF_Init");
		SDL_Quit();
		return 1;
	}

	SDL_Window *win = SDL_CreateWindow("Ronin", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if(win == nullptr)
	{
		logSDLError(std::cout, "CreateWindow");
		SDL_Quit();
		return 1;
	}

	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if(ren == nullptr){
		cleanup(win);
		logSDLError(std::cout, "CreateRenderer");
		SDL_Quit();
		return 1;
	}

  const std::string resPath = getResourcePath("Ronin");

  bool quit = false;
	SDL_Event e;

	while(!quit)
	{
		// read user input and handle it

		// e is an SDL_Event variable we've declared before entering the main loop
		while(SDL_PollEvent(&e))
		{
			// If user closes the window
			if(e.type == SDL_QUIT)
			{
				quit = true;
			}
			// if user presses any key
			if(e.type == SDL_KEYDOWN)
			{
				//use number input to select which clip should be drawn
				switch (e.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						quit = true;
						break;
					default:
						break;
				}
			}
			// If user clicks the mouse
			if(e.type == SDL_MOUSEBUTTONDOWN)
			{
				quit = true;
			}
		}

		// render our scene
		SDL_RenderClear(ren);
		//renderTexture(image, ren, x, y);
		SDL_RenderPresent(ren);
	}

	//cleanup(image, ren, win);
  cleanup(ren, win);
	TTF_Quit();
	SDL_Quit();

	return 0;
}
