#include <string>
#include <iostream>
#include "log.h"
#include "res_path.h"
#include "cleanup.h"
#include "sdl.h"

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
 * Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
 * the texture's width and height
 * @param tex The source texture we want to draw
 * @param ren The renderer we want to draw to
 * @param x The x coordinate to draw to
 * @param y The y coordinate to draw to
 */
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y)
{
	int w, h;
	SDL_QueryTexture(tex, NULL, NULL, &w, &h);
	renderTexture(tex, ren, x, y, w, h);
}

void set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    Uint8 *target_pixel = (Uint8 *)surface->pixels + y * surface->pitch + x * 4;
    *(Uint32 *)target_pixel = pixel;
}

/*
 * This is an implementation of the Midpoint Circle Algorithm
 * found on Wikipedia at the following link:
 *
 *   http://en.wikipedia.org/wiki/Midpoint_circle_algorithm
 *
 * The algorithm elegantly draws a circle quickly, using a
 * set_pixel function for clarity.
 */
void draw_circle(SDL_Surface *surface, int n_cx, int n_cy, int radius, Uint32 pixel)
{
    // if the first pixel in the screen is represented by (0,0) (which is in sdl)
    // remember that the beginning of the circle is not in the middle of the pixel
    // but to the left-top from it:

    double error = (double)-radius;
    double x = (double)radius -0.5;
    double y = (double)0.5;
    double cx = n_cx - 0.5;
    double cy = n_cy - 0.5;

    while (x >= y)
    {
        set_pixel(surface, (int)(cx + x), (int)(cy + y), pixel);
        set_pixel(surface, (int)(cx + y), (int)(cy + x), pixel);

        if (x != 0)
        {
            set_pixel(surface, (int)(cx - x), (int)(cy + y), pixel);
            set_pixel(surface, (int)(cx + y), (int)(cy - x), pixel);
        }

        if (y != 0)
        {
            set_pixel(surface, (int)(cx + x), (int)(cy - y), pixel);
            set_pixel(surface, (int)(cx - y), (int)(cy + x), pixel);
        }

        if (x != 0 && y != 0)
        {
            set_pixel(surface, (int)(cx - x), (int)(cy - y), pixel);
            set_pixel(surface, (int)(cx - y), (int)(cy - x), pixel);
        }

        error += y;
        ++y;
        error += y;

        if (error >= 0)
        {
            --x;
            error -= x;
            error -= x;
        }
    }
}

int main(int argc, char **argv)
{
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 460;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		logSDLError(std::cout, "SDL_INIT");
		return 1;
	}
	SDL_Window *win = SDL_CreateWindow("Bubble!", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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

	SDL_RenderClear(ren);

	/* Create a 32-bit surface with the bytes of each pixel in R,G,B,A order,
		 as expected by OpenGL for textures */
	SDL_Surface *surface;
	Uint32 rmask, gmask, bmask, amask;

	/* SDL interprets each pixel as a 32-bit number, so our masks must depend
	on the endianness (byte order) of the machine */
  #if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
	#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
	#endif

	surface = SDL_CreateRGBSurface(0, 640, 480, 32, rmask, gmask, bmask, amask);
	if(surface == NULL)
	{
		logSDLError(std::cout, "CreateRGBSurface failed");
		IMG_Quit();
		SDL_Quit();
		return 1;
	}

	draw_circle(surface, 100, 100, 20, SDL_MapRGB(format, 0xFF, 0xFC, 0x00 );

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
				quit = true;
			}
			// If user clicks the mouse
			if(e.type == SDL_MOUSEBUTTONDOWN)
			{
				quit = true;
			}
		}

		// render our scene
		/*
		SDL_RenderClear(ren);
		renderTexture(image, ren, x, y);
		SDL_RenderPresent(ren);
		*/
	}

	cleanup(surface, ren, win);
	IMG_Quit();
	SDL_Quit();

	return 0;
}
