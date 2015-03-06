# SDLDojo
SDL Training Ground

Doing the tutorial from TwinklebearDev:
http://www.willusher.io/pages/sdl2/

Installing SDL2 with homebrew is simple:
brew install SDL2

to find out paths for compiler and linker (see makefile) use
/usr/local/bin/sdl2-config --cflags
-I/usr/local/include/SDL2 -D_THREAD_SAFE
FlexAir:bin kemuri$ sdl2-config --libs
-L/usr/local/lib -lSDL2

Alternativly you can install the SDL2 Framework manually from the website.
Adapt Makefile accordingly. I wasn't able to make it run on OSX Yosemite,
maybe someone can help.
