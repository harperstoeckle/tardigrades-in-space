#include "shapes.hpp"

namespace gaforp
{
	point3 rotate_about_x(const point3& point, const point3& rotate_center, float angle)
	{
		//find the points' locations relative to the rotation center
		float relative_y = point.y - rotate_center.y;
		float relative_z = point.z - rotate_center.z;
		
		return {point.x,
				relative_y * std::cos(angle) - relative_z * std::sin(angle) + rotate_center.y,
				relative_y * std::sin(angle) + relative_z * std::cos(angle) + rotate_center.z};
	}
	
	point3 rotate_about_y(const point3& point, const point3& rotate_center, float angle)
	{
		//find the points' locations relative to the rotation center
		float relative_x = point.x - rotate_center.x;
		float relative_z = point.z - rotate_center.z;
		
		return {relative_z * std::sin(angle) + relative_x * std::cos(angle) + rotate_center.x,
				point.y,
				relative_z * std::cos(angle) - relative_x * std::sin(angle) + rotate_center.z};
	}
	
	point3 rotate_about_z(const point3& point, const point3& rotate_center, float angle)
	{
		//find the points' locations relative to the rotation center
		float relative_x = point.x - rotate_center.x;
		float relative_y = point.y - rotate_center.y;
		
		return {relative_x * std::cos(angle) - relative_y * std::sin(angle) + rotate_center.x,
				relative_x * std::sin(angle) + relative_y * std::cos(angle) + rotate_center.y,
				point.z};
	}
	
	point3 translate(const point3& point, const point3& move_vector)
	{
		return {point.x + move_vector.x,
				point.y + move_vector.y,
				point.z + move_vector.z};
	}
	
	hexahedron make_rprism(const point3& center, float half_width, float half_length, float half_height)
	{
		//the locations of the edges in a rectangular prism
		hexahedron prism{
			{0, 1}, {0, 2}, {1, 3}, {2, 3},
			{0, 4}, {1, 5}, {2, 6}, {3, 7},
			{4, 5}, {4, 6}, {5, 7}, {6, 7}};
		
		prism.set_vertices({
			{center.x - half_width, center.y - half_length, center.z - half_height},
			{center.x + half_width, center.y - half_length, center.z - half_height},
			{center.x - half_width, center.y + half_length, center.z - half_height},
			{center.x + half_width, center.y + half_length, center.z - half_height},
			{center.x - half_width, center.y - half_length, center.z + half_height},
			{center.x + half_width, center.y - half_length, center.z + half_height},
			{center.x - half_width, center.y + half_length, center.z + half_height},
			{center.x + half_width, center.y + half_length, center.z + half_height}});
		
		//please gabeN give me NRVO
		return prism;
	}
}