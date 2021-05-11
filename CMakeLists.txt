cmake_minimum_required(VERSION 3.1.2)
project(mysolver VERSION 0.1.0)

add_executable(mysolver src/main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

include_directories(thirdparty/include)
# link_directories(thirdparty/lib)
target_link_directories(mysolver PRIVATE thirdparty/lib)
target_link_libraries(mysolver GLEW)
target_link_libraries(mysolver glfw3)

add_subdirectory(thirdparty/src)
include_directories(thirdparty/src/tdogl)
target_link_libraries(mysolver tdogl)

macro(resourceCopier)  # this macro comes from Tom Dalling's tutorial
	file(COPY 
  		${CMAKE_CURRENT_SOURCE_DIR}/resources 
  		DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
	)
  if (WIN32)
    file(COPY 
        ${CMAKE_CURRENT_SOURCE_DIR}/resources 
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/${_TRG_NAME}.dir
    )
  endif ()
endmacro(resourceCopier)

resourceCopier()


IF(APPLE)  # MacOS requires a few extra libraries to make GLFW work
    include_directories(/System/Library/Frameworks)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(OPENGL_LIBRARY OpenGL)
    find_library(IOKIT_LIBRARY IOKit)
    target_link_libraries(mysolver ${COCOA_LIBRARY} ${OPENGL_LIBRARY} ${IOKIT_LIBRARY})
ENDIF (APPLE)
