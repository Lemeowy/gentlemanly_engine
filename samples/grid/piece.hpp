#ifndef GE_PIECE_HPP
#define GE_PIECE_HPP

#pragma once

#include <ge/actor.hpp>
#include "grid.hpp"

class piece : public ge::actor
{
protected:
	enum Directions; 
	Directions my_direction = NORTH;
public:
    enum Directions { NORTH = 0, EAST = 3, SOUTH = 2, WEST = 1 };
	grid* m_grid;
	int m_level;
	std::array<std::vector<piece*>, 4> checkNearbySquares(glm::ivec2 myLocation);
	std::vector<std::vector<piece*>> piece::squares_in_direction(glm::ivec2 myLocation, Directions direction, int range);
	void initialize(glm::ivec3 loc)
	{
		m_grid = static_cast<grid*>(get_parent());
		set_parent(m_grid);
		set_relative_location({float(loc.x), float(loc.y)});
		m_level = loc.z;
	}
	void set_grid_location(glm::ivec3 loc)
	{
		set_relative_location({loc.x, loc.y});
		m_level = loc.z;
	}
	glm::ivec3 get_grid_location() const
	{
		return {int(get_relative_location().x), int(get_relative_location().y), m_level};
	}
	void rotate(Directions direction)
	{
		set_relative_rotation(direction*glm::half_pi<float>());
		my_direction = direction;
	}
};

#endif  // GE_PIECE_HPP
