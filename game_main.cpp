#include <curses.h>

#include "shapes.hpp"
#include "transform.hpp"

#include "game_main.hpp"
#include "game_manager.hpp"

game_main::game_main(void):
	manager(std::make_unique<game_manager>())
{
	//setup ncurses
	initscr();
	//hide the cursor and don't echo the input
	curs_set(0);
	noecho();
	cbreak();
	
	//set the input map
	manager->add_key(KeyboardKey::escape, game_manager::QUIT);
	manager->add_key(KeyboardKey::left_arrow, game_manager::MOVE_LEFT);
	manager->add_key(KeyboardKey::right_arrow, game_manager::MOVE_RIGHT);
	
	//display the start screen
	mvprintw(static_cast<int>(manager->info.height_center), static_cast<int>(manager->info.actual_center) - 17, "you are the space tardigrade in space");
	mvprintw(static_cast<int>(manager->info.height_center)+2, static_cast<int>(manager->info.actual_center) - 17, "use arrow keys to move. avoid space trash");
	mvprintw(static_cast<int>(manager->info.height_center)+4, static_cast<int>(manager->info.actual_center) - 17, "press r to start the game");
	refresh();
	while(getch() != 'r')
	{}
}

game_main::~game_main(void)
{
	//end ncurses
	endwin();
}

bool game_main::should_close(void)
{
	return manager->should_close();
}

void game_main::process_input(void)
{
	manager->update_input();
}

void game_main::update(void)
{
	//check if the ship is in any planet hitboxes
	manager->update_hitbox();
	manager->update_timer(manager->countdown);
	manager->update_timer(manager->start);
	//make sure unseen planets are deleted
	manager->update_planets();

	
	//make a new object if the timer is done
	if(manager->timer_done(manager->countdown) && manager->timer_done(manager->start))
	{
		manager->add_planet({manager->get_random_screen() * manager->cube_side, 2.5, 0}, {manager->get_random_spin(), manager->get_random_spin(), manager->get_random_spin()}, 0.026);
		manager->set_timer(50, manager->countdown);
		manager->cube_side = -manager->cube_side;
		++manager->score;
	}
	
	//move objects down
	manager->move_planets();
	
	//move the ship
	manager->update_ship();
}

void game_main::render(void)
{
	erase();
	if(manager->is_dead)
	{
		manager->death_screen();
		return;
	}
	manager->draw_planets('#');
	manager->draw_ship('|');
	//tell the user how long they have spent in space
	mvprintw(2, static_cast<int>(manager->info.actual_center) + 60, "years in space: %u", manager->score);
	refresh();
}

bool game_main::should_restart(void)
{
	return manager->should_reset;
}

//g++ -ID:\libraries\include -LD:\libraries\lib -static-libgcc -static-libstdc++ game_loop.cpp game_main.cpp transform.cpp shapes.cpp -lpdcurses_32
//g++ -I. -L. -static-libgcc -static-libstdc++ game_loop.cpp game_main.cpp transform.cpp shapes.cpp -lpdcurses
