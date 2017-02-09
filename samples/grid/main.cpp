#include <ge/input_subsystem.hpp>
#include <ge/mesh_actor.hpp>
#include <ge/rocket_document_asset.hpp>
#include <ge/rocket_input_consumer.hpp>
#include <ge/rocket_subsystem.hpp>
#include <ge/runtime.hpp>
#include <ge/sdl_subsystem.hpp>
#include <ge/timer_subsystem.hpp>
#include <ge/renderable.hpp>

#include <glm/glm.hpp>

#include <cmath>
#include <memory>
#include "wall.hpp"
#include "grid.hpp"
#include "gridtick_interface.hpp"
#include "piece.hpp"
#include "turret.hpp"
#include "zombie.hpp"
#include "zombiespawner.hpp"
#include "hud.hpp"
#include "grid_rocket_element.hpp"
#include "spike.hpp"
#include "zombieupgrade.hpp"

#include <Rocket/Debugger/Debugger.h>

#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif

using namespace ge;

int main()
{
	runtime r;
	
	r.m_asset_manager.add_asset_path("data/");
	r.add_subsystem<input_subsystem>({});
	r.add_subsystem<timer_subsystem>({});
	auto& sdl = r.add_subsystem<sdl_subsystem>(sdl_subsystem::config{"Example!", {640, 960}});
	auto& rocket = r.add_subsystem<rocket_subsystem>({});

	r.register_interface<renderable>();
	r.register_interface<gridtick_interface>();

	// load UI
	auto doc = r.m_asset_manager.get_asset<rocket_document_asset>("gridui/doc.rocketdocument");
	doc->Show();
	
    auto griddoc = rocket.m_context->CreateDocument("body");
	griddoc->Show();
	
	auto pieceSelector = r.m_asset_manager.get_asset<rocket_document_asset>("gridui/piecebrowser.rocketdocument");
	pieceSelector->Show();
	pieceSelector->PullToFront();


	auto root = actor::root_factory(&r);

    auto camera = actor::factory<camera_actor>(root.get(), 16.5, float(sdl.get_size().x) / float(sdl.get_size().y));
    camera->set_relative_location(camera->get_relative_location() + glm::vec2(0, 2.75 ));

	sdl.set_background_color({.2f, .2f, .2f});
	sdl.set_camera(camera.get());
	r.set_root_actor(root.get());

	// initialize the grid
    auto g = actor::factory<grid>(root.get(), glm::ivec2{11, 11}, 20.f);
	actor::factory<zombieupgrade>(g.get(), glm::ivec3(-2,-2,-2));
	actor::factory<wall>(g.get(), glm::ivec2(4, 4));
	actor::factory<wall>(g.get(), glm::ivec2(4, 6));
	actor::factory<wall>(g.get(), glm::ivec2(6, 4));
	actor::factory<wall>(g.get(), glm::ivec2(6, 6));

	actor::factory<spike>(g.get(), glm::ivec2(3, 4));
	actor::factory<spike>(g.get(), glm::ivec2(3, 6));

	actor::factory<turret>(g.get(), glm::ivec2(4, 5), piece::WEST);
	actor::factory<turret>(g.get(), glm::ivec2(5, 6), piece::NORTH);
	actor::factory<turret>(g.get(), glm::ivec2(6, 5), piece::EAST);
	actor::factory<turret>(g.get(), glm::ivec2(5, 4), piece::SOUTH);
	
	for (int x = 0; x < 12; x++) {
		actor::factory<zombiespawner>(g.get(), glm::ivec2(-1, x));
		actor::factory<zombiespawner>(g.get(), glm::ivec2(11, x-1));
		actor::factory<zombiespawner>(g.get(), glm::ivec2(x-1, -1));
		actor::factory<zombiespawner>(g.get(), glm::ivec2(x, 11));
	}
	g->try_spawn_z();



	grid_rocket_instancer::registerInstancer();

    // create virtual rocket elements
    auto vp = camera->get_vp_matrix();
    auto tmpActor = actor::factory<piece>(g.get(), glm::ivec3(0, 0, 0));
    for(int x = 0; x < 11; ++x) {
        for(int y = 0; y < 11; ++y) {

            auto start = vp * tmpActor->calculate_model_matrix() * glm::vec3(x - .5, y + .5, 1);
            auto end = vp * tmpActor->calculate_model_matrix() * glm::vec3(x + .5, y + 1.5, 1);

			start += 1;
			start.x *= (float)sdl.get_size().x / 2.f;
			start.y *= (float)sdl.get_size().y / 2.f;

			end += 1;
			end.x *= (float)sdl.get_size().x / 2.f;
			end.y *= (float)sdl.get_size().y / 2.f;

            auto xml = Rocket::Core::XMLAttributes();
            xml.Set("idx", x);
            xml.Set("idy", y);
            xml.Set("start", Rocket::Core::Vector2f{start.x, sdl.get_size().y - start.y});
            xml.Set("size", Rocket::Core::Vector2f{end.x - start.x, end.y - start.y});

            auto elem = Rocket::Core::Factory::InstanceElement(nullptr, "grid_rocket", "grid_rocket", xml);
			griddoc->AppendChild(elem);

			auto str = "grid_" + std::to_string(x) + "_" + std::to_string(y);
			elem->SetId(str.c_str());
        }
    }

#ifndef WIN32
// 	Rocket::Debugger::Initialise(rocket.m_context);
// 	Rocket::Debugger::SetVisible(true);
#endif

	rocket_input_consumer ic{&r};
	ic.steal_input();

    actor::factory<hud>(root.get(), g.get(), doc.get());
    
#ifdef EMSCRIPTEN
	emscripten_set_main_loop_arg(
		[](void* run_ptr) {
			runtime* runt = (runtime*)run_ptr;

			runt->tick();
		},
		&r, 0, true);
#else
	while (r.tick())
		;
#endif
	
	int& a = *(int*)0;
}
