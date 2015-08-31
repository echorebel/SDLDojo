CXX = g++
# If your compiler is a bit older you may need to change -std=c++11 to -std=c++0x
SDL_LIB = $(shell sdl2-config --libs) #-L/usr/local/lib -lSDL2 -Wl,-rpath=/usr/local/lib
SDL_INCLUDE = $(shell sdl2-config --cflags) #-I/usr/local/include
CXXFLAGS = -Wall -c -std=c++11 $(SDL_INCLUDE) -Iinclude
LDFLAGS = $(SDL_LIB)
INITTEST = bin/SDLInitTest.out
PRINTRES = bin/PrintResPath.out
LESSON1 = bin/Lesson1.out
LESSON2 = bin/Lesson2.out


all: $(INITTEST) $(LESSON1) $(LESSON2)

$(INITTEST): SDLInitTest.o
	$(CXX) $< $(LDFLAGS) -o $@

SDLInitTest.o: src/SDLInitTest.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

$(LESSON1): Lesson1.o
	$(CXX) $< $(LDFLAGS) -o $@

Lesson1.o: src/Lesson1.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

$(LESSON2): Lesson2.o
	$(CXX) $< $(LDFLAGS) -o $@

Lesson2.o: src/Lesson2.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

print: $(PRINTRES)

$(PRINTRES): PrintResPath.o
	$(CXX) $< $(LDFLAGS) -o $@

PrintResPath.o: src/PrintResPath.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm *.o && rm $(INITTEST) && rm bin/*.out
