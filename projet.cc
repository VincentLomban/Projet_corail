#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

#include "simulation.h"



int main(int argc, char *argv[])
{

    Simulation simulation;
    if(argc != 2) exit(EXIT_FAILURE);
    
    simulation.lecture(argv[1], false);
        
    return EXIT_SUCCESS;
}
