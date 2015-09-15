#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include "res_path.h"
#include "cleanup.h"

/**
 * Log an SDL error with some error message to the output stream of your choice
 * @param os The output stream to write message to
 * @param msg The error message to write, format will be msg error: SDL_GetError()
**/
void logSDLError(std::ostream &os, const std::string &msg){
	os << msg << " error: " << SDL_GetError() << std::endl;
}

/**
 * Loads a BMP image into a texture on the rendering device
 * @param file The BMP image file to load
 * @param ren The renderer to load the texture onto
 * @return the loaded texture, or nullptr if something went wrong.
 */
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren){
	// Initialize to nullptr to avoid dangling pointer issues
	SDL_Texture *texture = nullptr;
	// Load the image
	SDL_Surface *loadedImage = SDL_LoadBMP(file.c_str());
	// If loading went ok, convert the texture and return the texture
	if(loadedImage != nullptr){
	       	texture = SDL_CreateTextureFromSurface(ren, loadedImage);
       		SDL_FreeSurface(loadedImage);
 		// Make sure converting went ok too
		if(texture == nullptr){
			logSDLError(std::cout, "CreateTextureFromSurface");
		}
	}
	else {
		logSDLError(std::cout, "LoadBMP");

	}

	return texture;
}
/**
 * Draw an SDL_Texture to an SDL_Rendere at position x, y, preserving
 * the texture's width and height
 * @param tex The source texture we want to draw
 * @param ren The renderer we want to draw to
 * @param x The x coordinate to draw to
 * @param y The y coordinate to draw to
 */
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y){
	//Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	//Query the texture to get its width and height to use
	SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
	int out;
	out = SDL_RenderCopy(ren, tex, NULL, &dst);
	if(out !=0){ logSDLError(std::cout, "RenderCopy"); }
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

	std::string resPath = getResourcePath("lesson2");
	SDL_Texture *background = loadTexture(resPath + "background.bmp", ren);
	SDL_Texture *image = loadTexture(resPath + "image.bmp", ren);
	if(background == nullptr || image == nullptr){
		cleanup(background, image, ren, win);
		logSDLError(std::cout, "LoadImages");
		SDL_Quit();
		return 1;
	}

	SDL_RenderClear(ren);
	int bW, bH;
	SDL_QueryTexture(background, NULL, NULL, &bW, &bH);
	printf("rect w %d h %d \n", bW, bH);
	renderTexture(background, ren, 0, 0);
	renderTexture(background, ren, bW, 0);
	renderTexture(background, ren, 0, bH);
	renderTexture(background, ren, bW, bH);

	int iW, iH;
	SDL_QueryTexture(image, NULL, NULL, &iW, &iH);
	int x = SCREEN_WIDTH / 2 - iW / 2;
	int y = SCREEN_HEIGHT / 2 - iH / 2;
	renderTexture(image, ren, x, y);

	SDL_RenderPresent(ren);

	SDL_Delay(5000);

	cleanup(background, image, ren, win);
	SDL_Quit();
	return 0;
}
