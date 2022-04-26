/**
 * This file contains implementations for methods in the VirtualAddress class.
 *
 * You'll need to add code here to make the corresponding tests pass.
 */

#include "virtual_address/virtual_address.h"

using namespace std;

VirtualAddress VirtualAddress::from_string(int process_id, string address) {
    // TODO: implement me
    return VirtualAddress(0, 0, 0);
}


string VirtualAddress::to_string() const {
    // TODO: implement me
    return "";
}


ostream& operator <<(ostream& out, const VirtualAddress& address) {
    // TODO: implement me
    return out;
}
