#ifndef VERTEX_HPP_INCLUDED
#define VERTEX_HPP_INCLUDED 1

#include <array>
#include <initializer_list>

namespace gaforp
{
	//a 3d point(stage 0)
	struct point3
	{
		float x;
		float y;
		float z;
	};
	
	//an edge between two vertices, described as two indices in the vertex array
	struct edge
	{
		std::size_t start;
		std::size_t end;
	};
	
	//copy as many as possible values from initializer_list into array, filling remaining spots with default initialized values
	template <typename T, std::size_t S>
	void safeish_copy(std::initializer_list<T> init, std::array<T, S>& ar)
	{
		auto init_start = std::begin(init);
		auto init_end = std::end(init);
		for(auto& i : ar)
		{
			if(init_start >= init_end)
			{
				i = T{};
				continue;
			}
			i = *init_start;
			++init_start;
		}
	}
	
	//contains the vertices and edges of a 3d structure.
	template <std::size_t VC, std::size_t EC>
	class net3
	{
	public:
		//the net3 is initialized with its edges
		net3(std::initializer_list<edge> edges)
		{
			safeish_copy(edges, edges_);
		}
		
		void set_vertices(std::initializer_list<point3> vertices)
		{
			safeish_copy(vertices, vertices_);
		}
		
		std::size_t edge_count(void) const
		{
			return EC;
		}
		
		std::size_t vertex_count(void) const
		{
			return VC;
		}
		
		//get nth vertex
		const point3& operator[](std::size_t n) const
		{
			return vertices_.at(n);
		}
		
		point3& operator[](std::size_t n)
		{
			return vertices_.at(n);
		}
		
		//get the actual points represented by the start and end of an edge
		const point3& get_edge_start(std::size_t n) const
		{
			return vertices_.at(edges_.at(n).start);
		}
		
		const point3& get_edge_end(std::size_t n) const
		{
			return vertices_.at(edges_.at(n).end);
		}
		
	private:
		std::array<point3, VC> vertices_;
		//this is effectively const, but can't be so that the constructor can copy elements into it
		std::array<edge, EC> edges_;
	};
	
	typedef net3<8, 12> hexahedron;
	//a 2d point(stage 1) this still faces upwards, and is translated into the weird upside-down screen coordinates later
	struct point2
	{
		float x;
		float y;
	};
	
	//a screen coordinate(stage 2)
	struct screen_coord
	{
		float x;
		float y;
	};
}

#endif