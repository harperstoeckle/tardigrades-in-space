#ifndef GAME_MAIN_HPP_INCLUDED
#define GAME_MAIN_HPP_INCLUDED 1

#include <memory>

//forward declaration for a game manager type, which manages the stuff in the game
class game_manager;

//all of the stuff in the game is contained in a class so there are "no" globals
class game_main
{
public:
	game_main(void);
	
	~game_main();
	
	bool should_close(void);
	bool should_restart(void);
	
	void process_input(void);
	
	void update(void);
	
	void render(void);
	
private:
	//this refers to the object which manages the stuff in the game
	std::unique_ptr<game_manager> manager;
};

#endif