# link_libraries()
# include_directories(${CMAKE_BINARY_DIR}/src)
include_directories(../src)
include_directories(../thirdparty/include)

# message(STATUS ${HEADER_FILES})

add_executable(testmain test-main.cpp test-algorithms.cpp ../src/Algorithms.cpp ../src/Particles.cpp ${HEADER_FILES} catch_amalgamated.cpp)
add_test(NAME MainTest COMMAND ./testmain)

# add_executable(testmain test-main.cpp catch_amalgamated.cpp)
# add_test(testmain ./testmain)