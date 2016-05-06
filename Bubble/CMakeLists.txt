project(Bubble)
find_package(SDL2_image REQUIRED)
include_directories(${SDL2_IMAGE_INCLUDE_DIR})
add_executable(Bubble src/main.cpp)
# link this lesson against SDL2 and SDL2_image
target_link_libraries(Lesson4 ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY})
install(TARGETS Bubble RUNTIME DESTINATION ${BIN_DIR})
