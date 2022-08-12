#ifndef TRANSFORM_HPP_INCLUDED
#define TRANSFORM_HPP_INCLUDED 1

#include "vertex.hpp"

//gaforp graphics pipeline: 3d verticies -> 2d vertices on screen -> screen-sized continuous coordinates -> edges drawn to screen

//these functions exist to transform data into different forms in the pipeline
namespace gaforp
{
	//information about the screen
	struct screen_info
	{
		point3 camera_pos;
		//the screen is conceptually a plane with equation z = screen_depth
		float screen_depth;
		//centers(as floats) of the screen's width and height
		float width_center;
		float height_center;
		//the screen's actual width, so it can be centered
		float actual_center;
		
	};
	
	//using the point and a camera, map the point to the imaginary screen
	point2 map_to_screen(const screen_info&, const point3&);
	
	//map to continuous screen coordinates using the screen size
	screen_coord map_to_coord(const screen_info&, const point2&);
	
	//draw a single screen coordinate to the actual screen
	void draw(const screen_coord&, char);
	
	//given two screen coordinates, draw an edges
	void draw_edge(const screen_coord&, const screen_coord&, char);
	
	//draw a single point onto the screen
	void draw_point(const screen_info&, const point3&, char);
	
	//draw an entire net3 (or at least its edges)
	template <std::size_t VC, std::size_t EC>
	void draw_net(const screen_info& info, const net3<VC, EC>& net, char shape)
	{
		for(int i=0; i<net.edge_count(); ++i)
		{
			auto start_coord = map_to_coord(info, map_to_screen(info, net.get_edge_start(i)));
			auto end_coord = map_to_coord(info, map_to_screen(info, net.get_edge_end(i)));
			draw_edge(start_coord, end_coord, shape);
		}
	}
}

#endif