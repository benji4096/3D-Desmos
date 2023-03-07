cmake_minimum_required(VERSION 3.8)

project("3D-Desmos")



##Include libraries
add_subdirectory(lib/glew-cmake) # OpenGL
add_subdirectory(lib/glfw) # window library
#add_subdirectory(lib/stbImplimentation) # Image library (not in use)
add_subdirectory(lib/glm) # Vector math
add_subdirectory(lib/freetype) # Font glyph library



##Include sub-projects.
add_subdirectory("Renderer")
add_subdirectory("Grapher3D")
add_subdirectory("main")



##Set working directory
set_property(TARGET main PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
