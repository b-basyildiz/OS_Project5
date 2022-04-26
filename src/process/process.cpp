/**
 * This file contains implementations for methods in the Process class.
 *
 * You'll need to add code here to make the corresponding tests pass.
 */

#include "process/process.h"

using namespace std;


Process* Process::read_from_input(std::istream& in) {
    // TODO
    return nullptr;
}


size_t Process::size() const
{
    // TODO
    return this->num_bytes;
}


bool Process::is_valid_page(size_t index) const
{
    // TODO
    return false;
}


size_t Process::get_rss() const
{
    // TODO
    return 0;
}


double Process::get_fault_percent() const
{
    // TODO
    return 0.0;
}
