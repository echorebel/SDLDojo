project(Lesson2)
add_executable(Lesson2 src/Lesson2.cpp)
target_link_libraries(Lesson2 ${SDL2_LIBRARY})
install(TARGETS Lesson2 RUNTIME DESTINATION ${BIN_DIR})
