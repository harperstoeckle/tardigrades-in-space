//this file does the main game loop at 100 ticks per second
#include <chrono>

//all of the main game functions are stored in a class called game_main, defined here
#include "game_main.hpp"

int main(void)
{

	
	using namespace std::literals::chrono_literals;
	
	//length of a tick in milliseconds
	constexpr auto TICK_LENGTH = 10ms;
	
	auto previous_tick = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
	auto lag = 0ms;
	
	bool again = 0;
	do
	{
	//make an instance of the game object, which should do initialization and cleanup in the constructor and destructor
	game_main game{};
	while(!game.should_close())
	{
		//set current frame start to now and find difference between frame start times in order to keep the game at a constant rate
		auto current_tick = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
		lag += current_tick - previous_tick;
		previous_tick = current_tick;
		
		//process and handle any input, setting stuff
		game.process_input();
		
		for(;lag > TICK_LENGTH; lag -= TICK_LENGTH)
		{
			game.update();
		}
		
		game.render();
	}
	again = game.should_restart();
	}while(again);
	return 0;
}

/*
gaforp::net3<4, 6> tetrahedron{{0, 1}, {0, 2}, {0, 3}, {1, 2}, {1, 3}, {2, 3}};
tetrahedron.set_vertices({{0, 1, 0}, {-0.866, -0.5, 0}, {0.866, -0.5, 0}, {0, 0, 1.414}});
*/