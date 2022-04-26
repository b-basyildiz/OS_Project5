/**
 * This file contains the main() function that drives the simulation. You'll
 * need to add logic to this file to create a Simulation instance and invoke its
 * run() method.
 */

#include <cstdlib>

#include "flag_parser/flag_parser.h"
#include "simulation/simulation.h"

using namespace std;

/**
* The main entry point to the simulation.
*/

int main(int argc, char** argv) {
    FlagOptions flags;

    bool error_flag = parse_flags(argc, argv, flags);

    if(error_flag == false) {
        print_usage();
        return 1;
    }

    Simulation sim(flags);

    int error = 0;
    error = sim.read_simulation_file();
    
    if (error) {
        std::cout << "ERROR READING FILE" << std::endl;
        return 1;
    }

    sim.run();
    
    return EXIT_SUCCESS;
}
