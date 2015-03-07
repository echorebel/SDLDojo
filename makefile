CXX = clang++
# If your compiler is a bit older you may need to change -std=c++11 to -std=c++0x
CXXFLAGS = -Wall -c -std=c++11 -I/usr/local/include/SDL2 -D_THREAD_SAFE
LDFLAGS = -L/usr/local/lib -lSDL2
EXE = bin/SDLInitTest.out

all: $(EXE)

$(EXE): SDLInitTest.o
	$(CXX) $(LDFLAGS) $< -o $@

SDLInitTest.o: SDLInitTest.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm *.o && rm $(EXE)
