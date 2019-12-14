#include "array.h"

array::array(uint64_t size, uint64_t *values) {
    Size=size;
    Array=values;
}

array::~array() {}

uint64_t array::countKeys(uint64_t start,const uint64_t *column) {
    uint64_t counter=1;                 //counts how many elements in the sorted array have the same value with the start element
    for(uint64_t i=start+1; i<Size; i++){
        if(column[Array[i]]!=column[Array[start]]) break;   //values are saved in array column
        counter++;
    }
    return counter;
}
