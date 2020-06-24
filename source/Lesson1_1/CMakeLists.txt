cmake_minimum_required(VERSION 3.8)

set(TARGET_NAME Lesson1_1)

find_package(OpenGL REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(gl3w REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

if(MSVC)
	set(PLATFORM_OPTION WIN32)
endif()

file(GLOB TARGET_SOURCE "*.h" "*.cpp")
add_executable(${TARGET_NAME} ${PLATFORM_OPTION} ${TARGET_SOURCE})

target_link_libraries(${TARGET_NAME} PRIVATE glm)
target_link_libraries(${TARGET_NAME} PRIVATE unofficial::gl3w::gl3w)
target_link_libraries(${TARGET_NAME} PRIVATE glfw)