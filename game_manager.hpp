#ifndef GAME_MANAGER_HPP_INCLUDED
#define GAME_MANAGER_HPP_INCLUDED 1

#include <vector>
#include <random>
#include <ctime>
#include <cmath>
#include <utility>

#include <windows.h>	//for GetKeyState(), which is not portable really
#undef MOUSE_MOVED

#include "shapes.hpp"
#include "transform.hpp"

//test whether a value lies between two values
inline bool is_between(float value, float low, float high)
{
	return value < std::max(low, high) && value > std::min(low, high);
}

//take the dot product of two point3s
inline float dot_product(const gaforp::point3& a, const gaforp::point3& b)
{
	return a.x * b.x + a.y * b.y + a.z + b.z;
}

//subtract first vector from second
inline gaforp::point3 vec_sub(const gaforp::point3& a, const gaforp::point3& b)
{
	return {a.x - b.x, a.y - b.y, a.z - b.z};
}

//holds an object and a center point
template <std::size_t VC, std::size_t EC>
struct center_object
{
	gaforp::net3<VC, EC> shape;
	gaforp::point3 center;
	//the spin of this is described as a point for some reason
	gaforp::point3 spin;
	//speed of the object downward
	float speed;
	
	//translate the object down
	void update(void)
	{
		gaforp::translate_points(shape, {0, -speed, 0});
		center = gaforp::translate(center, {0, -speed, 0});
		
		//update the spinniness
		gaforp::rotate_points_about_x(shape, center, spin.x);
		gaforp::rotate_points_about_y(shape, center, spin.y);
		gaforp::rotate_points_about_z(shape, center, spin.z);
	}
};

//gives distance between two points on the xy axis
inline float distance_xy(const gaforp::point3& a, const gaforp::point3& b)
{
	return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2));
}

//test whether a point lies within a hexahedron
template <std::size_t VC, std::size_t EC>
bool is_in_box(const gaforp::point3& point, const center_object<VC, EC>& box)
{
	/*
	auto u = vec_sub(box[0], box[1]);
	auto v = vec_sub(box[0], box[2]);
	auto w = vec_sub(box[0], box[4]);
	
	return is_between(dot_product(u, point), dot_product(u, box[0]), dot_product(u, box[1]))
			&& is_between(dot_product(v, point), dot_product(v, box[0]), dot_product(v, box[2]))
			&& is_between(dot_product(w, point), dot_product(w, box[0]), dot_product(w, box[4]));
	*/
	/*
	auto i = vec_sub(box[1], box[0]);
	auto j = vec_sub(box[2], box[0]);
	auto k = vec_sub(box[4], box[0]);
	auto v = vec_sub(point, box[0]);
	
	return 0.0f < dot_product(v, i) && dot_product(v, i) < dot_product(i, i)
			&& 0.0f < dot_product(v, j) && dot_product(v, j) < dot_product(j, j)
			&& 0.0f < dot_product(v, k) && dot_product(v, k) < dot_product(k, k);
	*/
	//the 3d stuff just doesn't work, so i'll just look in a circle
	return distance_xy(point, box.center) < distance_xy(box.shape[0], box.shape[1]) * 0.85;
}

struct ship
{
	//the ship is a triangle
	gaforp::net3<3, 3> ship;
	//this will be checked for the hitbox
	gaforp::point3 center;
	//the ship's angle
	float angle;
	//the speed at which the ship rolls
	float turn_speed;
	//the speed at which it strafes
	float strafe_speed;
	
	//continues turning and strafing or not depending on the limits set
	void update(float left_turn_limit, float right_turn_limit, float left_move_limit, float right_move_limit)
	{
		float possible_angle = angle + turn_speed;
		//only update if it's within bounds
		if(possible_angle <= left_turn_limit && possible_angle >= right_turn_limit)
		{
			angle = possible_angle;
			gaforp::rotate_points_about_y(ship, center, turn_speed);
		}
		
		float possible_move = center.x + strafe_speed;
		if(possible_move <= right_move_limit && possible_move >= left_move_limit)
		{
			center.x = possible_move;
			gaforp::translate_points(ship, {strafe_speed, 0, 0});
			
		}
	}
	//resets the movement stuff
	void reset_move(void)
	{
		strafe_speed = 0;
		turn_speed = 0;
	}
	
	//levels the ship at the given positive rate
	void level(float rate)
	{
		//if the angle is already zero, do nothing
		if(angle == 0.0f)
			return;
		
		//find the direction the ship should be turning to correct itself
		rate = angle > 0.0f? -std::abs(rate) : std::abs(rate);
		
		//if the difference is greater than the angle required to reach zero, just set it to zero
		if(std::abs(angle) < std::abs(rate))
		{
			gaforp::rotate_points_about_y(ship, center, -angle);
			angle = 0.0f;
			return;
		}
		
		angle += rate;
		gaforp::rotate_points_about_y(ship, center, rate);
	}
};

//swap and pop elements in a vector for efficient removal
template<typename T>
void swap_and_pop(std::vector<T>& vec, std::size_t index)
{
	if(index > vec.size() - 1)
		return;
	//only swap if the index is not at the last element
	if(index != vec.size() - 1)
		std::swap(vec[index], vec.back());
	
	vec.pop_back();
}

//check if a given key is being pressed currently
inline bool is_pressed(int key)
{
	//get the correct bit
	return GetKeyState(key) & 0x8000;
}

//this manages all of the stuff in the game
struct game_manager
{
	//stores the information about the screen, including camera position, screen size, and screen depth
	gaforp::screen_info info{{0, 0, 8}, 4, 50, 25, 90};
	
	//stores the interpreted state of the last input
	enum input_type
	{
		NONE,
		MOVE_LEFT,
		MOVE_RIGHT,
		QUIT
	};
	
	//stores an key-type pair, so that a key is mapped to an input_type
	struct map_input
	{
		int input_key;
		input_type input;
	};
	
	//last input
	input_type input = NONE;
	
	//pushes a new key combination. this is chainable
	game_manager& add_key(int input_key, input_type input)
	{
		keys_.push_back({input_key, input});
		return *this;
	}
	
	//update the input value
	void update_input(void)
	{
		for(auto key : keys_)
		{
			//finds the first valid key and sets it
			if(is_pressed(key.input_key))
			{
				input = key.input;
				return;
			}
		}
		//if no valid keys found, set it to NONE, as none are pressed
		input = NONE;
	}
	
	//get a random number
	float get_random_screen(void)
	{
		return generate_random_(random_generator_);
	}
	
	float get_random_spin(void)
	{
		return random_spin_(random_generator_);
	}
	
	//set up some random stuff
	game_manager(void):
		random_generator_(std::time(nullptr)),
		generate_random_(0, 2.0f),
		random_spin_(-0.02, 0.02),
		ship_({{{0,1},{0,2},{1,2}}, {0, -1.6, 0}, 0, 0, 0})
	{
		//setup the ship
		ship_.ship.set_vertices({{0, -1.3, 0}, {-0.4, -1.7, 0}, {0.4, -1.7, 0}});
	}
	
	//sets the timer for a given amount of frames
	void set_timer(std::size_t frames, std::size_t& timer)
	{
		timer = frames;
	}
	
	//checks if the timer has run out
	bool timer_done(std::size_t& timer)
	{
		return !timer;
	}
	
	//updates the state of the timer. will not decrease if there is still stuff going on
	void update_timer(std::size_t& timer)
	{
		if(timer)
			--timer;
	}
	
	//adds a planet to the planet vector at the given point
	void add_planet(const gaforp::point3& location, const gaforp::point3& spin, float speed)
	{
		float planet_size = 0.1 + get_random_screen()/4;
		planets_.push_back({gaforp::make_rprism(location, planet_size, planet_size, planet_size), location, spin, speed});
	}
	
	//deletes planets that are not on the screen, possibly far
	void update_planets(void)
	{
		for(std::size_t i = 0; i < planets_.size(); ++i)
		{
			if(planets_[i].center.y < -3.0f || planets_[i].center.y > 3.0f)
				swap_and_pop(planets_, i);
		}
	}
	 
	//move the planets down
	void move_planets(void)
	{
		for(auto& planet : planets_)
			planet.update();
	}
	 
	//draw the planets
	void draw_planets(char shape)
	{
		for(const auto& planet : planets_)
			gaforp::draw_net(info, planet.shape, shape);
	}
	 
	//draw the ship
	void draw_ship(char shape)
	{
		gaforp::draw_net(info, ship_.ship, shape);
		//draw the center of the ship
		gaforp::draw_point(info, ship_.center, '@');
	}
	
	//update the movement of the ship
	void update_ship(void)
	{
		switch(input)
		{
		case QUIT:
			should_close_ = 1;
		case MOVE_LEFT:
			ship_.strafe_speed = -0.03;
			ship_.turn_speed = 0.05;
			break;
		case MOVE_RIGHT:
			ship_.strafe_speed = 0.03;
			ship_.turn_speed = -0.05;
			break;
		default:	//also make sure to slowly turn it back
			ship_.reset_move();
			ship_.level(0.02 + std::abs(ship_.angle)/50);
		}
		
		ship_.update(1.2, -1.2, -1.9, 1.9);
	}
	
	//update the death flag based on whether the point is in a hitbox
	void update_hitbox(void)
	{
		//if it is in any planet, death is here
		for(const auto& planet : planets_)
		{
			if(is_in_box(ship_.center, planet))
			{
				is_dead = 1;
				return;
			}
		}
		is_dead = 0;
	}
	
	//is the player dead?
	bool is_dead = 0;
	
	//side of the screen to draw the cube on
	float cube_side = 1.0f;
	
	//is the game over?
	bool should_close(void)
	{
		return should_close_;
	}
	
	//set that the game should close
	void set_should_close(void)
	{
		should_close_ = 1;
	}
	
	//show this screen on death
	void death_screen(void)
	{
		mvprintw(static_cast<int>(info.height_center), static_cast<int>(info.actual_center) - 17, "you died after %u years drifting in space", score);
		mvprintw(static_cast<int>(info.height_center) + 2, static_cast<int>(info.actual_center) - 17, "press r to restart and anything else to quit");
		refresh();
		char got = getch();
		should_close_ = 1;
		if(got == 'r')
			should_reset = 1;
	}
	
	bool should_reset = 0;
	
	//the player's score
	std::size_t score= 0;
	
	//holds the current value in a countdown timer
	std::size_t countdown = 0;
	
	std::size_t start = 300;
	
	
private:
	//holds all key maps
	std::vector<map_input> keys_;
	
	//these are used to generate random numbers within the view area
	std::mt19937 random_generator_;
	std::uniform_real_distribution<float> generate_random_;
	std::uniform_real_distribution<float> random_spin_;
	
	//vector of objects to draw
	std::vector<center_object<8, 12>> planets_;
	
	//the ship
	ship ship_;
	
	//should the game close?
	bool should_close_ = 0;
};

#endif