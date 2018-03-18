//#include "sdl.h"
#include <SDL2/SDL.h>
#include <iostream>

#ifdef __APPLE__
   #include <SDL_ttf.h>
   #include <SDL_image.h>
#elif __linux__
   #include <SDL2/SDL_ttf.h>
   #include <SDL2/SDL_image.h>
#endif

#include "log.h"

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
}
