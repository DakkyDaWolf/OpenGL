cmake_minimum_required(VERSION 3.8)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

project (OpenGL_Essentials_LiveLessons)

if(MSVC)
	add_compile_options(/W4 /WX /std:c++17)
else()
	add_compile_options(-Wall -Wextra -pedantic -Werror -std=c++17 -Wno-ignored-qualifiers)
endif()

add_subdirectory("source")