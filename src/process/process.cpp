/**
 * This file contains implementations for methods in the Process class.
 *
 * You'll need to add code here to make the corresponding tests pass.
 */

#include "process/process.h"

using namespace std;


Process* Process::read_from_input(std::istream& in) {
    //TODO 
    size_t num_bytes = in.gcount(); 
    vector<Page*> pages; 
    string input; 
    while(!in.eof()){
        Page* curr_page = Page::read_from_input(in); 
        pages.push_back(curr_page); 
    }
    Process* process = new Process(num_bytes,pages); //Why error? 
    return process; 

}


size_t Process::size() const
{   
    //TODO
    int size = 0; 
    for(int i = 0; i < pages.size(); ++i){
        size += pages[i]->size();
    }
    return size; 
}


bool Process::is_valid_page(size_t index) const
{
    // TODO
   if(index < pages.size()){
       return true;
   }
   return false;
}

size_t Process::get_rss() const
{
    // TODO
    return this->page_table.get_present_page_count();
}


double Process::get_fault_percent() const
{
    // TODO
    if(memory_accesses == 0){
        return 0;
    }
    return float(page_faults)/memory_accesses * 100; 
}
