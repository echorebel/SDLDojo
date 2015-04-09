#include <iostream>
#include <string>
#include <SDL.h>
#include "res_path.h"
#include "cleanup.h"

/**
https://wiki.libsdl.org/SDL_GetBasePath
return working folder where binary runs, this is the data directory
related function is https://wiki.libsdl.org/SDL_GetPrefPath, where u can store user data / save games
**/

int main(int argc, char **argv)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0){
		std::cout << "SDL_Init error: " << SDL_GetError() << std::endl;
		return 1;
	}
	//std::cout << "Resource path is: " << getResourcePath() << std::endl;

	SDL_Window *win = SDL_CreateWindow("Hello World!", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
	if(win == nullptr)
	{
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}
	
	SDL_Renderer *ren = SDL_CreateRenderer
	(
		win, -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);
	if(ren == nullptr){
		SDL_DestroyWindow(win);
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
	}
	
	std::string imagePath = getResourcePath("lesson1") + "hello.bmp";
	SDL_Surface *bmp = SDL_LoadBMP(imagePath.c_str());
	if(bmp == nullptr){
		cleanup(ren, win);
		std::cout << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, bmp);
	SDL_FreeSurface(bmp);
	if(tex == nullptr){
		std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	SDL_RenderClear(ren);
	SDL_RenderCopy(ren, tex, NULL, NULL);
	SDL_RenderPresent(ren);

	SDL_Delay(2000);

	cleanup(tex, ren, win);	
	SDL_Quit();
	return 0;
}
