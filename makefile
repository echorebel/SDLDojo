CXX = clang++
# If your compiler is a bit older you may need to change -std=c++11 to -std=c++0x
CXXFLAGS = -Wall -c -std=c++11 -I/usr/local/include/SDL2 -Iinclude -D_THREAD_SAFE
LDFLAGS = -L/usr/local/lib -lSDL2
INITTEST = bin/SDLInitTest.out
PRINTRES = bin/PrintResPath.out
LESSON1 = bin/Lesson1.out

all: $(INITTEST) $(LESSON1)

$(LESSON1): Lesson1.o
	$(CXX) $(LDFLAGS) $< -o $@

Lesson1.o: src/Lesson1.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

$(INITTEST): SDLInitTest.o
	$(CXX) $(LDFLAGS) $< -o $@

SDLInitTest.o: src/SDLInitTest.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

print: $(PRINTRES)

$(PRINTRES): PrintResPath.o
	$(CXX) $(LDFLAGS) $< -o $@

PrintResPath.o: src/PrintResPath.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm *.o && rm $(INITTEST) && rm bin/*.out

