#pragma once

#include <ge/actor.hpp>
#include <ge/mesh_actor.hpp>
#include <ge/texture_asset.hpp>
#include "grid.hpp"
#include "gridtick_interface.hpp"
#include "piece.hpp"

class zombie : public piece
{
public:
	ge::mesh_actor* m_mesh;

	void initialize(glm::uvec3 location)
	{
		piece::initialize(location);

		add_interface<zombie, gridtick_interface>();

		m_mesh = factory<ge::mesh_actor>(this, "texturedmodel/textured.meshsettings").get();
		m_mesh->m_mesh_settings.m_material.m_property_values["Texture"] =
			m_runtime->m_asset_manager.get_asset<ge::texture_asset>("zombie.texture");
	}
	void move_closer_to_center()
	{
		glm::uvec2 gridCenter = { m_grid->get_size().x / 2, m_grid->get_size().y / 2 };
		glm::uvec2 myLocation = get_relative_location();
		glm::uvec2 wayToGo = gridCenter - myLocation;
		if (wayToGo.x < 0)
		{
			myLocation.x--;
		}
		else if (wayToGo.x >0)
		{
			myLocation.x++;
		}

		if (wayToGo.y < 0)
		{
			myLocation.y--;
		}
		else if (wayToGo.y >0)
		{
			myLocation.y++;
		}
		set_relative_location(myLocation);
	}
	void tick_grid() { std::cout << "Ticked!\n"; }
};
