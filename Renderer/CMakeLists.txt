
cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

##Library directories
SET(LIBRARY_INCLUDE_DIRECTORIES
	${PROJECT_SOURCE_DIR}/lib/glew-cmake/include/
	${PROJECT_SOURCE_DIR}/lib/glfw/include/
	${PROJECT_SOURCE_DIR}/lib/glm/glm/
	${PROJECT_SOURCE_DIR}/lib/freetype/include/
)
SET(LIBRARY_LINK_OBJECTS
	libglew_static
	glfw
	glm
	freetype
)

##Setting source and include directories
SET(SOURCE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src)
SET(INCLUDE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include)

##Source Files
SET(SOURCE_FILES
	${SOURCE_DIRECTORY}/Renderer.cpp
	${SOURCE_DIRECTORY}/Shader.cpp
	${SOURCE_DIRECTORY}/Mesh.cpp
	${SOURCE_DIRECTORY}/glUtils.cpp
)
##Header Files
SET(INCLUDE_FILES
	${INCLUDE_DIRECTORY}/Renderer.hpp
	${INCLUDE_DIRECTORY}/Shader.hpp
	${INCLUDE_DIRECTORY}/Mesh.hpp
	${INCLUDE_DIRECTORY}/glUtils.h
)

##Add library
add_library(Renderer STATIC ${SOURCE_FILES} ${INCLUDE_FILES})
target_include_directories(Renderer PUBLIC ${INCLUDE_DIRECTORY} ${LIBRARY_INCLUDE_DIRECTORIES})
target_link_libraries(Renderer PUBLIC ${LIBRARY_LINK_OBJECTS})
