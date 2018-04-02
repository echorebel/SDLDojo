cmake_minimum_required(VERSION 2.6)
project(SDLDojo)

# use our modified FindSDL2* modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${SDLDojo_SOURCE_DIR}/cmake")
# set an output directory for our binaries
set(BIN_DIR ${SDLDojo_SOURCE_DIR}/bin)

# Bump warning levels appropiatly for clang, gcc & msvc
# Also set debug/optimization flags depending on the build type. IDE users choose this when
# selecting the build mode in their IDE
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=c++11")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} -g")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} -02")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUEL "MSVC")
	if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
		string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	else()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
	endif()
endif()

# Look up SDL2 and add the include directory to our include path
find_package(SDL2 REQUIRED)
include_directories(${SDL_INCLUDE_DIR})
include_directories(include)

# Look in the Lessons subdirectory to find its CMakeLists.txt so we can build the executables
add_subdirectory(Lesson0)
add_subdirectory(Lesson1)
add_subdirectory(Lesson2)
add_subdirectory(Lesson3)
add_subdirectory(Lesson4)
add_subdirectory(Lesson5)
add_subdirectory(Lesson6)
add_subdirectory(Bubble)
