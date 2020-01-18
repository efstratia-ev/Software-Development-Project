#include "rows_array.h"

rows_array::rows_array(uint64_t size, uint64_t *values) {
    Size=size;
    Array=values;
    rows=NULL;
}


uint64_t rows_array::countKeys(uint64_t start,const uint64_t *column) {
    uint64_t counter=1;                 //counts how many elements in the sorted rows_array have the same value with the start element
    for(uint64_t i=start+1; i<Size; i++){
        if(rows && column[rows[Array[i]]]!=column[rows[Array[start]]]) break;
        if(!rows && column[Array[i]]!=column[Array[start]]) break;   //values are saved in rows_array column
        counter++;
    }
    return counter;
}

rows_array::rows_array(uint64_t size, uint64_t *values, uint64_t *rs) {
    Size=size;
    Array=values;
    rows=rs;
}

uint64_t rows_array::get_value(uint64_t row) {
    if(rows) return rows[Array[row]];
    else return Array[row];
}

rows_array::~rows_array() {
 //   delete Array;
}

