#pragma once

// Windows
#if defined(_WIN32)
#if !defined(NOMINMAX)
#define NOMINMAX
#endif
#include <windows.h>
#endif

// Standard
#include <cstdlib>
#include <exception>
#include <stdexcept>
#include <cassert>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <cstdint>
#include <sstream>
#include <fstream>
#include <chrono>
#include <functional>
#include <algorithm>
#include <iterator>

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

// Guidline Support Library
#include <gsl/gsl>

// OpenGL
#include <GL/gl3w.h>
#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

#if defined(_LINUX)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#endif

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#if defined(_LINUX)
#pragma GCC diagnostic pop
#endif

//#include <SOIL/SOIL.h>

// Assimp
#if defined(_LINUX)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#endif
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#if defined(_LINUX)
#pragma GCC diagnostic pop
#endif

// Local
#include "ServiceContainer.h"

namespace Library
{
	extern ServiceContainer GlobalServices;
}