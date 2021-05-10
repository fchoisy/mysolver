cmake_minimum_required(VERSION 3.0.0)
project(mysolver VERSION 0.1.0)

add_executable(mysolver src/main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

# find_package(GLEW REQUIRED)
# find_package(glfw3 3.3 REQUIRED)
# find_package(glm REQUIRED)

# target_include_directories(mysolver PRIVATE ${GLEW_INCLUDE_DIRS})
# target_link_libraries(mysolver PRIVATE ${GLEW_LIBRARIES})
# target_include_directories(mysolver PRIVATE ${glfw3_INCLUDE_DIRS})
# target_link_libraries(mysolver PRIVATE ${glfw3_LIBRARIES})
# target_include_directories(mysolver PRIVATE ${glm_INCLUDE_DIRS})
# target_link_libraries(mysolver PRIVATE ${glm_LIBRARIES})
# message(<DEBUG> ${GLEW_INCLUDE_DIRS})

include_directories(thirdparty/include)
# link_directories(thirdparty/lib)
target_link_directories(mysolver PRIVATE thirdparty/lib)
target_link_libraries(mysolver GLEW)
target_link_libraries(mysolver glfw3)

IF(APPLE)
    include_directories(/System/Library/Frameworks)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(OPENGL_LIBRARY OpenGL)
    find_library(IOKIT_LIBRARY IOKit)
    target_link_libraries(mysolver ${COCOA_LIBRARY} ${OPENGL_LIBRARY} ${IOKIT_LIBRARY})

#    FIND_LIBRARY(CARBON_LIBRARY Carbon)
#    FIND_LIBRARY(QUICKTIME_LIBRARY QuickTime )
#    FIND_LIBRARY(APP_SERVICES_LIBRARY ApplicationServices )
#    MARK_AS_ADVANCED (CARBON_LIBRARY
#                      QUICKTIME_LIBRARY
#                      APP_SERVICES_LIBRARY)
#    SET(EXTRA_LIBS ${CARBON_LIBRARY} ${QUICKTIME_LIBRARY} ${APP_SERVICES_LIBRARY})
ENDIF (APPLE)
