/**
 * main
 * 
 * Entry point to the program.
 * Starts an Experiment and catches all exceptions.
 */

#include "BoundaryExperiment.hpp"
#include <iostream> // std::cerr

int main(int argc, char *argv[])
{
    try
    {
        BoundaryExperiment boundaryExperiment;
        boundaryExperiment.Run();
    }
    catch (const std::exception &e)
    {
        std::cerr << "ERROR caught at highest level: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
