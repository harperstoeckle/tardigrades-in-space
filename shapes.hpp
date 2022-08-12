#ifndef SHAPES_HPP_INCLUDED
#define SHAPES_HPP_INCLUDED 1

#include <cmath>
#include "vertex.hpp"

//provides facilities for creating and manipulating shapes
namespace gaforp
{
	//generate a rectangular prism with the given center, length, width, and height
	hexahedron make_rprism(const point3& center, float half_width, float half_length, float half_height);
	
	//rotate a point about axis, passing through a point
	point3 rotate_about_x(const point3& point, const point3& rotate_center, float angle);

	point3 rotate_about_y(const point3& point, const point3& rotate_center, float angle);

	point3 rotate_about_z(const point3& point, const point3& rotate_center, float angle);
	
	//translate a point by a vector(described as a point)
	point3 translate(const point3& point, const point3& move_vector);

	//rotate a net about an axis
	template <std::size_t VC, std::size_t EC>
	void rotate_points_about_x(net3<VC, EC>& poly, const point3& rotate_center, float angle)
	{
		for(std::size_t i=0; i<VC; ++i)
			poly[i] = rotate_about_x(poly[i], rotate_center, angle);
	}

	template <std::size_t VC, std::size_t EC>
	void rotate_points_about_y(net3<VC, EC>& poly, const point3& rotate_center, float angle)
	{
		for(std::size_t i=0; i<VC; ++i)
			poly[i] = rotate_about_y(poly[i], rotate_center, angle);
	}

	template <std::size_t VC, std::size_t EC>
	void rotate_points_about_z(net3<VC, EC>& poly, const point3& rotate_center, float angle)
	{
		for(std::size_t i=0; i<VC; ++i)
			poly[i] = rotate_about_z(poly[i], rotate_center, angle);
	}
	
	template <std::size_t VC, std::size_t EC>
	void translate_points(net3<VC, EC>& poly, const point3& move_vector)
	{
		for(std::size_t i=0; i<VC; ++i)
			poly[i] = translate(poly[i], move_vector);
	}
}


#endif