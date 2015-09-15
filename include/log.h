#include <SDL2/SDL.h>

/**
 * Log an SDL error with some error message to the output stream of your choice
 * @param os The output stream to write message to
 * @param msg The error message to write, format will be msg error: SDL_GetError()
**/
void logSDLError(std::ostream &os, const std::string &msg){
	os << msg << " error: " << SDL_GetError() << std::endl;
}
