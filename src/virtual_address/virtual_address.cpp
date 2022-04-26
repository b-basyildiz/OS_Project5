/**
 * This file contains implementations for methods in the VirtualAddress class.
 *
 * You'll need to add code here to make the corresponding tests pass.
 */

#include "virtual_address/virtual_address.h"

//My additions
#include <string>
#include <sstream>
#include <ostream>
#include <stdlib.h> 
#include <iostream>
#include <math.h>

using namespace std;

VirtualAddress VirtualAddress::from_string(int process_id, string address) {
    // TODO: implement me
    int page = 0;
    int offset = 0;
    for(int i = 0; i < 10; ++i){
        if(address.at(i) == '1'){
            page += pow(2,9-i); 
        }
    }
    for(int i = 10; i < 16; ++i){
        if(address.at(i) == '1'){
            offset += pow(2,15-i); 
        }
    }
    return VirtualAddress(process_id, page, offset);
}


string VirtualAddress::to_string() const {
    // TODO: implement me
    //little unsure about this
    string out1 = "";

    int temp_page = this->page; 
    for(int i = 0; i < this->PAGE_BITS; ++i){
        ostringstream ss; 
        ss << temp_page % 2;
        out1 = ss.str() + out1; 
        ss.clear(); 
        temp_page /= 2;
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


ostream& operator <<(ostream& out, const VirtualAddress& address) {
    // TODO: implement me
    
    out << "PID " + to_string(address.process_id) + " @ " +  address.to_string() + " [page: " + to_string(address.page) + "; offset: " + to_string(address.offset) +"]";
    return out;
}
