cmake_minimum_required(VERSION 3.8)

set(TARGET_NAME Lesson5_4)

find_package(OpenGL REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(gl3w REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(SOIL CONFIG REQUIRED)

if(MSVC)
	set(PLATFORM_OPTION WIN32)
endif()

file(GLOB TARGET_SOURCE "*.h" "*.cpp")
add_executable(${TARGET_NAME} ${PLATFORM_OPTION} ${TARGET_SOURCE})

include_directories("../Library.Shared")

target_link_libraries(${TARGET_NAME} PRIVATE ${OPENGL_gl_LIBRARY})
target_link_libraries(${TARGET_NAME} PRIVATE glm)
target_link_libraries(${TARGET_NAME} PRIVATE unofficial::gl3w::gl3w)
target_link_libraries(${TARGET_NAME} PRIVATE glfw)
target_link_libraries(${TARGET_NAME} PRIVATE Library)
target_link_libraries(${TARGET_NAME} PRIVATE ${ASSIMP_LIBRARIES})
target_link_libraries(${TARGET_NAME} PRIVATE SOIL)

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/Content" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${LIBRARY_CONTENT}" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")