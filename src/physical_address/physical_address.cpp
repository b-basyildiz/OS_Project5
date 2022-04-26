/**
 * This file contains implementations for methods in the PhysicalAddress class.
 *
 * You'll need to add code here to make the corresponding tests pass.
 */

#include "physical_address/physical_address.h"

#include <ostream>
#include <sstream>
#include <string>

using namespace std;

string PhysicalAddress::to_string() const {
    // TODO: implement me
    string out1 = "";

    int temp_frame = this->frame; 
    for(int i = 0; i < this->FRAME_BITS; ++i){
        ostringstream ss; 
        ss << temp_frame % 2;
        out1 = ss.str() + out1; 
        ss.clear(); 
        temp_frame /= 2;
    }

    int temp_offset = this->offset;
    string out2 = "";
    for(int i = 0; i < this->OFFSET_BITS; ++i){
        ostringstream ss; 
        ss << temp_offset % 2;
        out2 = ss.str() + out2; 
        ss.clear(); 
        temp_offset /= 2;
    }


    return out1 + out2; 
}


ostream& operator <<(ostream& out, const PhysicalAddress& address) {
    // TODO: implement me
    out << address.to_string() + " [frame: " + to_string(address.frame) + "; offset: " + to_string(address.offset) + "]";
    return out;
}
