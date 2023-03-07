
cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

##Library directories
SET(LIBRARY_INCLUDE_DIRECTORIES
	${PROJECT_SOURCE_DIR}/Grapher3D/include/
)
SET(LIBRARY_LINK_OBJECTS
	Grapher3D
)

##Setting source and include directories
SET(SOURCE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/)
SET(INCLUDE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/)
##Source Files
SET(SOURCE_FILES
	${SOURCE_DIRECTORY}/main.cpp
)
##Header Files
SET(INCLUDE_FILES
)

##Add executable
add_executable(main ${SOURCE_FILES} ${INCLUDE_FILES})
target_include_directories(main PUBLIC ${INCLUDE_DIRECTORY} ${LIBRARY_INCLUDE_DIRECTORIES})
target_link_libraries(main PUBLIC ${LIBRARY_LINK_OBJECTS})
