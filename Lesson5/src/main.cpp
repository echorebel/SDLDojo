#include <string>
#include <iostream>
#include <SDL2/SDL.h>
#ifdef __APPLE__
	#include <SDL_image.h>
#elif __linux__
	#include <SDL2/SDL_image.h>
#endif
#include "log.h"
#include "res_path.h"
#include "cleanup.h"

/**
 * Load an image into a texture on the rendering device
 * @param file The image file to load
 * @param ren The renderer to load the texture onto
 * @return the loaded texture, or nullptr if something went wrong.
 */
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren)
{
    SDL_Texture *texture = IMG_LoadTexture(ren, file.c_str());
    if(texture == nullptr)
    {
      logSDLError(std::cout, "LoadTexture");
    }
    return texture;
}

/**
 * Draw an SDL_Texture to and SDL_Renderer at position x, y, with some desired
 * width and height
 * @param tex The source texture we want to draw
 * @param ren The renderer we want to draw to
 * @param x The x coordinate to draw to
 * @param y The y coordinate to draw to
 * @param w The width of the texture to draw
 * @param h The height of the texture to draw
 */
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h)
{
	// Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = w;
	dst.h = h;
	SDL_RenderCopy(ren, tex, NULL, &dst);
}

/**
 * Draw an SDL_Texture to an SDL_Renderer at some destination rect
 * taking a clip of the texture if desired
 * @param tex The source texture we want to draw
 * @param ren The renderer we want to draw to
 * @param dst The destination rectangle to render the texture to
 * @param clip The sub-section of the texture to draw (clipping rect)
 * 		default of nullptr draws the entire texture
 */
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst, SDL_Rect *clip = nullptr)
{
	SDL_RenderCopy(ren, tex, clip, &dst);
}

/**
 * Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
 * the texture's width and height and taking a clip of the texture if desired
 * If a clip is passed, the clip's width and height will be used instead of
 * the texture's
 * @param tex The source texture we want to draw
 * @param ren The renderer we want to draw to
 * @param x The x coordinate to draw to
 * @param y The y coordinate to draw to
 * @param clip The sub-section of the texture to draw (clipping rect)
 * 	default of nullptr draws the entire texture
 */
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, SDL_Rect *clip = nullptr)
{
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	if(clip != nullptr)
	{
		dst.w = clip->w;
		dst.h = clip->h;
	} else {
		SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
	}
	renderTexture(tex, ren, dst, clip);
}

int main(int argc, char **argv)
{
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 460;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		logSDLError(std::cout, "SDL_INIT");
		return 1;
	}
	SDL_Window *win = SDL_CreateWindow("Hello World!", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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

	// not necessary happens automatically but it will delay start 
	// so it's better to have it under control
	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
	{
		logSDLError(std::cout, "IMG_Init");
		SDL_Quit();
		return 1;
	}

	const std::string resPath = getResourcePath("lesson5");
	SDL_Texture *image = loadTexture(resPath + "image.png", ren);
	// make sure they both loaded
	if(image == nullptr)
	{
		cleanup(image, ren, win);
		IMG_Quit();
		SDL_Quit();
		return 1;
	}	

	//iW and iH are the clip width and height
	int iW = 100, iH = 100;
	int x = SCREEN_WIDTH / 2 - iW / 2;
	int y = SCREEN_HEIGHT / 2 - iH / 2;
	renderTexture(image, ren, x, y);

	// setup clips for our image
	SDL_Rect clips[4];
	for (int i = 0; i < 4; ++i)
	{
		clips[i].x = i / 2 * iW;
		clips[i].y = i % 2 * iH;
		clips[i].w = iW;
		clips[i].h =iH;
	}
	// Specify a default clip to start with
	int useClip = 0;

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
					case SDLK_1:
						useClip = 0;
						break;
					case SDLK_2:
						useClip = 1;
						break;
					case SDLK_3:
						useClip = 2;
						break;
					case SDLK_4:
						useClip = 3;
						break;
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
		renderTexture(image, ren, x, y, &clips[useClip]);
		SDL_RenderPresent(ren);
	}

	cleanup(image, ren, win);
	IMG_Quit();
	SDL_Quit();

	return 0;
}
