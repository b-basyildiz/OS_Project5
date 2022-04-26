/**
 * This file contains implementations for methods in the PageTable class.
 *
 * You'll need to add code here to make the corresponding tests pass.
 */

#include "page_table/page_table.h"

using namespace std;


size_t PageTable::get_present_page_count() const {
    // TODO: implement me
    size_t count = 0;
    for(int i = 0; i < this->rows.size(); ++i){
        if(this->rows[i].present == true){
            ++count;
        }
    } 
    return count;
}


size_t PageTable::get_oldest_page() const {
    // TODO: implement me
    int time = -1;
    int index = -1;
    for(int i = 0; i < this->rows.size(); ++i){
        if(this->rows[i].present == true && this->rows[i].loaded_at != -1){
            if(time == -1){
                time = this->rows[i].loaded_at;
                index = i;
            }else if(this->rows[i].loaded_at < time){
                time = this->rows[i].loaded_at;
                index = i;
            }
        }
    }
    return index;
}


size_t PageTable::get_least_recently_used_page() const {
    // TODO: implement me
    int time = -1;
    int index = -1;
    for(int i = 0; i < this->rows.size(); ++i){
        if(this->rows[i].present == true && this->rows[i].last_accessed_at != -1){
            if(time == -1){
                time = this->rows[i].last_accessed_at;
                index = i;
            }else if(this->rows[i].last_accessed_at < time){
                time = this->rows[i].last_accessed_at;
                index = i;
            }
        }
    }
    return index; 
}
