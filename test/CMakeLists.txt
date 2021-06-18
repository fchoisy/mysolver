include_directories(../src)
include_directories(../thirdparty/include)

# message(STATUS ${HEADER_FILES})

# add_subdirectory(lib/Catch2)
add_executable(testmain test-main.cpp test-algorithms.cpp ../src/Algorithms.cpp ../src/Particles.cpp ../src/ParticleSimulation.cpp ${HEADER_FILES} catch_amalgamated.cpp)


# add_executable(tests test.cpp)
# target_link_libraries(testmain PRIVATE Catch2::Catch2)

add_test(NAME MainTest COMMAND ./testmain)

# add_executable(testmain test-main.cpp catch_amalgamated.cpp)
# add_test(testmain ./testmain)