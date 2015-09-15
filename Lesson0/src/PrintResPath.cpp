#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include "res_path.h"

/**
https://wiki.libsdl.org/SDL_GetBasePath
return working folder where binary runs, this is the data directory
related function is https://wiki.libsdl.org/SDL_GetPrefPath, where u can store user data / save games
**/

int main(int argc, char **argv){
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		std::cerr << "SDL_Init error: " << SDL_GetError() << std::endl;
		return 1;
	}
	std::cout << "Resource path is: " << getResourcePath() << std::endl;

	SDL_Quit();
	return 0;
}
