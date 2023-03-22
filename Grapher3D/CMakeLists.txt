
cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

##Library directories
SET(LIBRARY_INCLUDE_DIRECTORIES
	${PROJECT_SOURCE_DIR}/Renderer/include
)
SET(LIBRARY_LINK_OBJECTS
	Renderer
)

##Setting source and include directories
SET(SOURCE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src)
SET(INCLUDE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include)

##Source Files
SET(SOURCE_FILES
	${SOURCE_DIRECTORY}/Grapher3D.cpp
	${SOURCE_DIRECTORY}/MathExpression.cpp
)
##Header Files
SET(INCLUDE_FILES
	${INCLUDE_DIRECTORY}/Grapher3D.hpp
	${INCLUDE_DIRECTORY}/MathExpression.hpp
)

##Add library
add_library(Grapher3D STATIC ${SOURCE_FILES} ${INCLUDE_FILES})
target_include_directories(Grapher3D PUBLIC ${INCLUDE_DIRECTORY} ${LIBRARY_INCLUDE_DIRECTORIES})
target_link_libraries(Grapher3D PUBLIC ${LIBRARY_LINK_OBJECTS})
