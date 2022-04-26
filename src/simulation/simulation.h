/**
 * This file contains the definition of the Simulation class.
 *
 * You're free to modify this class however you see fit. Add new methods to help
 * keep your code modular.
 */

#pragma once
#include "process/process.h"
#include "virtual_address/virtual_address.h"
#include "flag_parser/flag_parser.h"
#include "frame/frame.h"
#include "physical_address/physical_address.h"


#include <map>
#include <list>
#include <fstream>
#include <cstdlib>
#include <iostream>
#include <cassert>
#include <boost/format.hpp>


/**
* Class responsible for running the memory simulation. 
*/

class Simulation {
public:

    //===================================
    // Member Functions
    //===================================

    /**
    * Runs the simulation.
    */
    void run();

    /**
    * The constructor.
    */
    Simulation(FlagOptions& flags);

    /**
    * Performs a memory access for the given virtual address, translating it to
    * a physical address and loading the page into memory if needed. Returns the
    * byte at the given address.
    */
    char perform_memory_access(const VirtualAddress& address);

    /**
    * Handles a page fault, attempting to load the given page for the given
    * process into memory.
    */
    void handle_page_fault(Process* process, size_t page);

    /**
    * Prints information about the simulation.
    */
    void print_summary();

    /**
    * Functions for reading in a simulation.
    */
    int read_simulation_file();
    int read_addresses(std::istream& simulation_file);
    int read_processes(std::istream& simulation_file);
    
    //===================================
    // Member Variables
    //===================================

    /**
    * The maximum number of frames in the simulated system (512).
    */
    static const size_t NUM_FRAMES = 1 << 9;

    /**
    * A map of processes included in this simulation, keyed by their PIDs.
    */
    std::map<int, Process*> processes;

    /**
    * The flags that were input to the command line when the simulation
    * was invoked.
    */
    FlagOptions flags;

    /**
    * The virtual memory addresses that represent the sequence of memory accesses
    * to be simulated.
    */
    std::vector<VirtualAddress> virtual_addresses;

    // std::ifstream simulation_file;

    /**
    * A vector of frames constituting the entirety of main memory.
    */
    std::vector<Frame> frames;

    /**
    * The total number of page faults that occurred.
    */
    size_t page_faults = 0;

    /**
    * A list containing the indices of all frames that are not currently in use.
    */
    std::list<size_t> free_frames;
};
