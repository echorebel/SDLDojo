#ifndef SDL_UTIL_H
#define SDL_UTIL_H
#endif

#include <string>
#include <iostream>
#include <SDL2/SDL.h>

#ifdef __APPLE__
   #include <SDL_ttf.h>
   #include <SDL_image.h>
#elif __linux__
   #include <SDL2/SDL_ttf.h>
   #include <SDL2/SDL_image.h>
#endif

#include "log.h"

using namespace std;

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
      logSDLError(cout, "LoadTexture");
   }
   return texture;
}

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
      logSDLError(cout, "TTF_OpenFont");
      return nullptr;
   }
   // We need to first render to a surface as that's what TTF_RenderText
   // return, then load that surface into a texture
   SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), color);
   if(surf == nullptr)
   {
      TTF_CloseFont(font);
      logSDLError(cout, "TTF_RenderText");
      return nullptr;
   }
   SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
   if(texture == nullptr)
   {
      logSDLError(cout, "CreateTexture");
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
 *       default of nullptr draws the entire texture
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
 *    default of nullptr draws the entire texture
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
