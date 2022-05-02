/**
 * This file contains implementations for the methods defined in the Simulation
 * class.
 *
 * You'll probably spend a lot of your time here.
 */

#include "simulation/simulation.h"
#include <stdexcept>
#include <iostream>
#include <string>

Simulation::Simulation(FlagOptions& flags)
{
    this->flags = flags;
    this->frames.reserve(this->NUM_FRAMES);
    
}

void Simulation::run() {
    // TODO: implement me
  for(int i = 0; i < 512; ++i){//actually made the values the frames 
    this->free_frames.push_back(i); 
  }

    for(int  i = 0; i < this->virtual_addresses.size(); ++i){
        this->perform_memory_access(virtual_addresses[i]);
        if(this->seg_fault){
            break;
        }
        if(flags.verbose){
            std::cout << "\t-> RSS: " << this->processes[this->virtual_addresses[i].process_id]->get_rss() << std::endl;
        }
    }
    if(!this->seg_fault){
        this->print_summary(); 
    }
    //how to deallocate memory of pages? 
    /*
   for(int  i = 0; i < this->virtual_addresses.size(); ++i){
       for(int j = 0; j < this->processes[virtual_addresses[i].process_id]->pages.size(); ++j){
           delete this->processes[virtual_addresses[i].process_id]->pages[i];
       }
    }
    */

    for(auto process:this->processes){
        for(auto page: process.second->pages){
            delete page;
        }
        delete process.second; 
    }
    
   
}
//ask about what to return. Is it the frame? or the page offset?
char Simulation::perform_memory_access(const VirtualAddress& virtual_address) {
    // TODO: implement me

    if(this->flags.verbose){
        std::cout << virtual_address << std::endl;
    }

    this->time++;
    this->processes[virtual_address.process_id]->memory_accesses += 1;
    if(!this->processes[virtual_address.process_id]->is_valid_page(virtual_address.page)){
        this->seg_fault = true;
        std::cout << "SEGFAULT - INVALID PAGE" << std::endl;
        exit(-1); 
    }

    PhysicalAddress p(this->processes[virtual_address.process_id]->page_table.rows[virtual_address.page].frame,virtual_address.offset);
    if(this->processes[virtual_address.process_id]->page_table.rows[virtual_address.page].present){//page is linked to frame
        this->processes[virtual_address.process_id]->page_table.rows[virtual_address.page].last_accessed_at = time;

        
        if(!this->processes[virtual_address.process_id]->pages[virtual_address.page]->is_valid_offset(virtual_address.offset)){
            this->seg_fault = true;
            std::cout << "SEGFAULT - INVALID OFFSET" << std::endl;
            exit(-1);
        }
        
        //PhysicalAddress p(this->processes[virtual_address.process_id]->page_table.rows[virtual_address.page].frame,virtual_address.offset);
        if(this->flags.verbose){
                std::cout << "\t-> IN MEMORY" <<std::endl;
                std::cout << "\t-> physical address " << p << std::endl; 
        }
        //return this->processes[virtual_address.process_id]->pages[virtual_address.page]->get_byte_at_offset(virtual_address.offset); 
    }else{
        //page does not have a frame 
        processes[virtual_address.process_id]->page_faults += 1;
        this->page_faults++;
        this->temp_offset = virtual_address.offset;
        this->handle_page_fault(this->processes[virtual_address.process_id],virtual_address.page);
    }
    //return this->processes[virtual_address.process_id]->pages[virtual_address.page]->get_byte_at_offset(virtual_address.offset); 
    return this->frames.at(p.frame).contents->get_byte_at_offset(virtual_address.offset); 
}

void Simulation::handle_page_fault(Process* process, size_t page) {
    // TODO: implement me
    int p_replace; 
    int f_num; 

    if(this->flags.verbose){
        std::cout << "\t-> PAGE FAULT" << std::endl;
    }

    if(process->page_table.get_present_page_count() < this->flags.max_frames){
            Frame f;
            f.set_page(process,page); 
            this->frames.push_back(f); 
            this->free_frames.pop_front();
            f_num = this->frame_count; 
            this->frame_count++; 

            p_replace = process->page_table.get_present_page_count(); 
    }else{
        //replacement policy 
        if (int(this->flags.strategy) == 0){//FIFO
            p_replace = process->page_table.get_oldest_page(); 
        }else{
            p_replace = process->page_table.get_least_recently_used_page();
        }
        f_num = process->page_table.rows[p_replace].frame;
        process->page_table.rows[p_replace].present = false; 
    }
    process->page_table.rows[page].present = true;
    process->page_table.rows[page].loaded_at = this->time; 
    process->page_table.rows[page].last_accessed_at = this->time; 
    process->page_table.rows[page].frame = f_num;
    this->frames.at(f_num).set_page(process,page); 

    if(this->flags.verbose){
        //TODO
        PhysicalAddress p(process->page_table.rows[page].frame,this->temp_offset);
        std::cout << "\t-> physical address " << p << std::endl; 
    }
    if(!process->pages[page]->is_valid_offset(this->temp_offset)){
        std::cout << "SEGFAULT - INVALID OFFSET" << std::endl;
        this->seg_fault = true;
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
