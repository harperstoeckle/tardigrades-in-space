cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(tardigrades LANGUAGES CXX)

add_executable(tardigrades
	game_loop.cpp
	game_main.hpp
	game_main.cpp
	game_manager.hpp
	input.hpp
	shapes.hpp
	shapes.cpp
	transform.hpp
	transform.cpp
	vertex.hpp)

find_package(Curses REQUIRED)
find_package(X11 REQUIRED)
target_link_libraries(tardigrades PUBLIC ${CURSES_LIBRARIES} ${X11_LIBRARIES})
