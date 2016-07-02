#ifndef GE_SDL_WINDOW_HPP
#define GE_SDL_WINDOW_HPP

#pragma once

#include "ge/concept/window.hpp"

#include <boost/optional.hpp>

#include <boost/signals2.hpp>

#include <glm/glm.hpp>

class SDL_Window;
using SDL_GLContext = void*;

namespace ge
{
class sdl_application;
class sdl_window;

class sdl_window
{
	SDL_GLContext m_context;
	glm::vec4 m_background_color;
	sdl_application* sdl_inst;

public:
	using application = sdl_application;
	using window = sdl_window;

	SDL_Window* m_window;

	const application& get_application() const { return *sdl_inst; }
	application& get_application() { return *sdl_inst; }
	sdl_window(sdl_application& sdl, const char* title, boost::optional<glm::uvec2> loc,
		glm::uvec2 size, bool fullscreen, bool decorated);

	// can't really copy OpenGL context, so don't allow copying of windows
	sdl_window(const sdl_window&) = delete;

	// move constructor
	sdl_window(sdl_window&& other)
		: m_window{other.m_window}, m_context{other.m_context}, sdl_inst{other.sdl_inst}
	{
		// invalidate `other`
		other.m_window = nullptr;
		other.m_context = nullptr;
		other.sdl_inst = nullptr;
	}

	// again, can't copy a context, no copy operator
	sdl_window& operator=(const sdl_window&) = delete;

	// move operator
	sdl_window& operator=(sdl_window&& other)
	{
		this->m_window = other.m_window;
		this->m_context = other.m_context;

		// invalidate other
		other.m_window = nullptr;
		other.m_context = nullptr;

		return *this;
	}

	~sdl_window();

	void execute();

	void set_title(const char* new_title);
	std::string get_title() const;

	void set_size(glm::uvec2 new_size);
	glm::uvec2 get_size() const;

	boost::signals2::signal<void()> sig_quit;
};
BOOST_CONCEPT_ASSERT((ge::concept::Window<sdl_window>));

}  // namespace ge

#endif // GE_SDL_WINDOW_HPP
