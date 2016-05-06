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

/**
 * Render the message we want to display to a texture for drawing
 * @param message The message we want to display
 * @param fontFile The font we want to use to render the text
 * @param color The color we want the text to be
 * @param fontSize The size we want the font to be
 * @param renderer The renderer to load the texture in
 * @return An SDL_Texture containing the rendered message, or nullptr if something went wrong
 */
SDL_Texture* renderText(const std::string &message, const std::string &fontfile, SDL_Color color, int fontSize, SDL_Renderer *renderer)
{
	// Open the font
	TTF_Font *font = TTF_OpenFont(fontfile.c_str(), fontSize);
	if(font == nullptr)
	{
		logSDLError(std::cout, "TTF_OpenFont");
		return nullptr;
	}
	// We need to first render to a surface as that's what TTF_RenderText
	// return, then load that surface into a texture
	SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), color);
	if(surf == nullptr)
	{
		TTF_CloseFont(font);
		logSDLError(std::cout, "TTF_RenderText");
		return nullptr;
	}
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
	if(texture == nullptr)
	{
		logSDLError(std::cout, "CreateTexture");
	}
	// clean surface and font
	SDL_FreeSurface(surf);
	TTF_CloseFont(font);
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
	if(ren == nullptr)
	{
		cleanup(win);
		logSDLError(std::cout, "CreateRenderer");
		SDL_Quit();
		return 1;
	}

  const std::string resPath = getResourcePath("Ronin");
	// Color is in RGBA format
	SDL_Color color = {255,255,255,255};
	SDL_Texture *image = renderText("Ronin", resPath + "kenvector_future.ttf", color, 64, ren);
	if(image == nullptr)
	{
		cleanup(ren, win);
		TTF_Quit();
		SDL_Quit();
		return 1;
	}

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

		// get texture w/h so we can enter it in the screen
		int iW, iH;
		SDL_QueryTexture(image, NULL,NULL, &iW, &iH);
		int x = SCREEN_WIDTH / 2 - iW / 2;
		int y = SCREEN_HEIGHT / 2 - iH / 2;

		// render our scene
		SDL_RenderClear(ren);
		renderTexture(image, ren, x, y);
		SDL_RenderPresent(ren);
	}

	cleanup(image, ren, win);
	TTF_Quit();
	SDL_Quit();

	return 0;
}
