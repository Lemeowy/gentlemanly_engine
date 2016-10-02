#ifndef GE_GL_HPP
#define GE_GL_HPP

#pragma once

#if defined(__APPLE__)
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#elif defined(__linux__)
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glu.h>
#elif defined(WIN32) || defined(_WIN32)
#include <glad/glad.h>
#endif

#endif
