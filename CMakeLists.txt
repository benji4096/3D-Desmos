cmake_minimum_required(VERSION 3.8)

project("3D-Desmos")



##Include libraries
#add_subdirectory(lib/glew-cmake) # OpenGL
#add_subdirectory(lib/SDL) # will probably change to glfw # window library
#add_subdirectory(lib/stbImplimentation) # Image loader
#add_subdirectory(lib/glm) # Vector math
#add_subdirectory(lib/freetype) # Font library



##Include sub-projects.
add_subdirectory("main")
