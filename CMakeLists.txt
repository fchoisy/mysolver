cmake_minimum_required(VERSION 3.1.2)
project(mysolver VERSION 0.1.0)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add source files
file(GLOB_RECURSE SOURCE_FILES 
	${CMAKE_SOURCE_DIR}/src/*.c
	${CMAKE_SOURCE_DIR}/src/*.cpp)
	
# Add header files
file(GLOB_RECURSE HEADER_FILES 
	${CMAKE_SOURCE_DIR}/src/*.h
	${CMAKE_SOURCE_DIR}/src/*.hpp)

configure_file(src/helpers/RootDir.h.in src/helpers/RootDir.h)
include_directories(${CMAKE_BINARY_DIR}/src)
include_directories(${CMAKE_BINARY_DIR}/src/imgui)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})


include_directories(thirdparty/include)
target_link_directories(mysolver PRIVATE thirdparty/lib)
target_link_libraries(mysolver GLEW)
target_link_libraries(mysolver glfw3)

add_subdirectory(thirdparty/src)
target_link_libraries(mysolver tdogl)

IF(APPLE)  # MacOS requires a few extra libraries to make GLFW work
    include_directories(/System/Library/Frameworks)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(OPENGL_LIBRARY OpenGL)
    find_library(IOKIT_LIBRARY IOKit)
    target_link_libraries(mysolver ${COCOA_LIBRARY} ${OPENGL_LIBRARY} ${IOKIT_LIBRARY})
ENDIF (APPLE)

enable_testing()
add_subdirectory(test)