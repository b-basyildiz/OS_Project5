/**
 * This file contains implementations for the methods defined in the Simulation
 * class.
 *
 * You'll probably spend a lot of your time here.
 */

#include "simulation/simulation.h"
#include <stdexcept>
#include <iostream>

Simulation::Simulation(FlagOptions& flags)
{
    this->flags = flags;
    this->frames.reserve(this->NUM_FRAMES);
    
}

void Simulation::run() {
    // TODO: implement me
    /*
    if(int(this->flags.strategy) == 0){//FIFO

    }else if(int(this->flags.strategy) == 0){//LRU

    }else{
        throw("Error: invalid replacement policy"); 
    }
    */ 
   /*
   for(auto itr = this->processes.begin(); itr != this->processes.end(); ++itr){
       int i = 0;
       for (auto page: this->processes[itr->first]->pages){
           //std::cout << this->virtual_addresses[i] << std::endl;
           std::cout << i << std::endl;
           ++i; 
       }
   }
   */
  for(int i = 0; i < 512; ++i){//actually made the values the frames 
    this->free_frames.push_back(i); 
  }

    for(int  i = 0; i < this->virtual_addresses.size(); ++i){
        //std::cout << this->virtual_addresses[i].process_id << std::endl;
        if(!this->perform_memory_access(virtual_addresses[i])){
            this->handle_page_fault(this->processes[this->virtual_addresses[i].process_id],virtual_addresses[i].page);
        }
    }
   this->print_summary(); 
}

char Simulation::perform_memory_access(const VirtualAddress& virtual_address) {
    // TODO: implement me
    //Thought process -> use process to find if the page is valid 
    //if it is, return 1 else return 0
    this->time++;
    processes[virtual_address.process_id]->memory_accesses += 1;
    if(!this->processes[virtual_address.process_id]->is_valid_page(virtual_address.page)){
        throw("Segmentation Fault: Accessing invalid page"); 
    }
    if(this->processes[virtual_address.process_id]->page_table.rows[virtual_address.page].present){ 
        this->processes[virtual_address.process_id]->page_table.rows[virtual_address.page].last_accessed_at = time;
        //std::cout << "Loaded page" << virtual_address.page << std::endl;
        return 1;
    }
    //std::cout << "Not loaded page" << std::endl;
    processes[virtual_address.process_id]->page_faults += 1;
    this->page_faults++;
    return 0; 
}

void Simulation::handle_page_fault(Process* process, size_t page) {
    // TODO: implement me
    for(int i = 0; i < this->flags.max_frames; ++i){
        if(!process->page_table.rows[i].present){
            process->page_table.rows[i].present = true;
            process->page_table.rows[i].loaded_at = this->time; 
            process->page_table.rows[i].last_accessed_at = this->time; 
            //process->page_table.rows[i].frame = i + 
            this->free_frames.pop_back();//we need to fix this as well
        }
    }
}

void Simulation::print_summary() {
    if (!this->flags.csv) {
        boost::format process_fmt(
            "Process %3d:  "
            "ACCESSES: %-6lu "
            "FAULTS: %-6lu "
            "FAULT RATE: %-8.2f "
            "RSS: %-6lu\n");

        for (auto entry : this->processes) {
            std::cout << process_fmt
                % entry.first
                % entry.second->memory_accesses
                % entry.second->page_faults
                % entry.second->get_fault_percent()
                % entry.second->get_rss();
        }

        // Print statistics.
        boost::format summary_fmt(
            "\n%-25s %12lu\n"
            "%-25s %12lu\n"
            "%-25s %12lu\n");

        std::cout << summary_fmt
            % "Total memory accesses:"
            % this->virtual_addresses.size()
            % "Total page faults:"
            % this->page_faults
            % "Free frames remaining:"
            % this->free_frames.size();
    }

    if (this->flags.csv) {
        boost::format process_fmt(
            "%d,"
            "%lu,"
            "%lu,"
            "%.2f,"
            "%lu\n");

        for (auto entry : processes) {
            std::cout << process_fmt
                % entry.first
                % entry.second->memory_accesses
                % entry.second->page_faults
                % entry.second->get_fault_percent()
                % entry.second->get_rss();
        }

        // Print statistics.
        boost::format summary_fmt(
            "%lu,,,,\n"
            "%lu,,,,\n"
            "%lu,,,,\n");

        std::cout << summary_fmt
            % this->virtual_addresses.size()
            % this->page_faults
            % this->free_frames.size();
    }
}

int Simulation::read_processes(std::istream& simulation_file) {
    int num_processes;
    simulation_file >> num_processes;

    for (int i = 0; i < num_processes; ++i) {
        int pid;
        std::string process_image_path;

        simulation_file >> pid >> process_image_path;

        std::ifstream proc_img_file(process_image_path);

        if (!proc_img_file) {
            std::cerr << "Unable to read file for PID " << pid << ": " << process_image_path << std::endl;
            return 1;
        }
        this->processes[pid] = Process::read_from_input(proc_img_file);
    }
    return 0;
}

int Simulation::read_addresses(std::istream& simulation_file) {
    int pid;
    std::string virtual_address;

    try {
        while (simulation_file >> pid >> virtual_address) {
            this->virtual_addresses.push_back(VirtualAddress::from_string(pid, virtual_address));
        }
    } catch (const std::exception& except) {
        std::cerr << "Error reading virtual addresses." << std::endl;
        std::cerr << except.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Error reading virtual addresses." << std::endl;
        return 1;
    }
    return 0;
}

int Simulation::read_simulation_file() {
    std::ifstream simulation_file(this->flags.filename);
    // this->simulation_file.open(this->flags.filename);

    if (!simulation_file) {
        std::cerr << "Unable to open file: " << this->flags.filename << std::endl;
        return -1;
    }
    int error = 0;
    error = this->read_processes(simulation_file);

    if (error) {
        std::cerr << "Error reading processes. Exit: " << error << std::endl;
        return error;
    }

    error = this->read_addresses(simulation_file);

    if (error) {
        std::cerr << "Error reading addresses." << std::endl;
        return error;
    }

    if (this->flags.file_verbose) {
        for (auto entry: this->processes) {
            std::cout << "Process " << entry.first << ": Size: " << entry.second->size() << std::endl;
        }

        for (auto entry : this->virtual_addresses) {
            std::cout << entry << std::endl;
        }
    }

    return 0;
}
