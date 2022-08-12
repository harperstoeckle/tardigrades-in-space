#include <cmath>
#include "transform.hpp"
#include <curses.h>

namespace gaforp
{
	point2 map_to_screen(const screen_info& info, const point3& point)
	{
		//calculate the value of t in the resulting parametric equation
		float t_position = (info.screen_depth - info.camera_pos.z) / (point.z - info.camera_pos.z);
		
		return {(point.x - info.camera_pos.x) * t_position + info.camera_pos.x,
				(point.y - info.camera_pos.y) * t_position + info.camera_pos.y};
	}
	
	screen_coord map_to_coord(const screen_info& info, const point2& screen_point)
	{
		//coordinates are funky, so deal with that. also move it over horizontally enough to be correct
		return {screen_point.x * info.width_center + info.actual_center,
				info.height_center - screen_point.y * info.height_center};
	}
	
	void draw(const screen_coord& coord, char shape)
	{
		//if the number is less than zero, truncate it to zero
		int y_coord = static_cast<int>(coord.y);
		int x_coord = static_cast<int>(coord.x);
		mvaddch(y_coord, x_coord, shape);
	}
	
	float sign(float value)
	{
		return std::signbit(value)? -1.0f : 1.0f;
	}
	
	void draw_edge(const screen_coord& start, const screen_coord& end, char shape)
	{
		float delta_x = end.x - start.x;
		float delta_y = end.y - start.y;
		
		//if both dy and dx are zero, it is just a point and should be drawn as such
		if(delta_x == 0.0f && delta_y == 0.0f)
		{
			draw({start.x, start.y}, shape);
			return;
		}
		
		//if abs(dx) > abs(dy), x is independent in the graph
		if(std::abs(delta_x) > std::abs(delta_y))
		{
			//rate of change
			float rate = delta_y / delta_x;
			
			float direction = sign(delta_x);
			//check against negatives if dx is negative
			for(float pos = 0.0f; pos * direction <= delta_x * direction; pos += direction)
				draw({pos + start.x, pos * rate + start.y}, shape);
			return;
		}
		
		//rate of change
		float rate = delta_x / delta_y;
		
		float direction = sign(delta_y);
		//check against negatives if dy is negative
		for(float pos = 0.0f; pos * direction <= delta_y * direction; pos += direction)
			draw({pos * rate + start.x, pos + start.y}, shape);
	}
	
	void draw_point(const screen_info& info, const point3& point, char shape)
	{
		draw(map_to_coord(info, map_to_screen(info, point)), shape);
	}
}