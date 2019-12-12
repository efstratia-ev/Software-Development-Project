#include "array.h"

array::array(uint64_t size, uint64_t *values) {
    Size=size;
    Array=values;
}

array::~array() {
    delete[] Array;
}

uint64_t array::countKeys(uint64_t start,uint64_t *column) {
    uint64_t counter=1;
    for(uint64_t i=start+1; i<Size; i++){
        if(column[Array[i]]!=column[Array[start]]) break;
        counter++;
    }
    return counter;
}
