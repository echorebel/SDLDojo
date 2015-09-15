#include <string>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "log.h"

SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren)
{
    SDL_Texture *texture = IMG_LoadTexture(ren, file.c_str());
    if(texture == nullptr)
    {
      logSDLError(std::cout, "LoadTexture");
    }
    return texture;
}

int main(int argc, char **argv)
{
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 460;

  const int TILE_SIZE = 40;

	return 0;
}
