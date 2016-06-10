project(Ronin)
find_package(SDL2_ttf REQUIRED SDL2_image REQUIRED)
include_directories(${SDL2_TTF_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR})
set( project_sources
	src/main.cpp
	src/Spaceship.cpp
)
add_executable(${PROJECT_NAME}
	${project_sources}
)
target_link_libraries(Ronin ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2_TTF_LIBRARY})
install(TARGETS Ronin RUNTIME DESTINATION ${BIN_DIR})
