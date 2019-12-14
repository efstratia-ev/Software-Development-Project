#include "array.h"

array::array(uint64_t size, uint64_t *values) {
    Size=size;
    Array=values;
    rows=NULL;
}


uint64_t array::countKeys(uint64_t start,const uint64_t *column) {
    uint64_t counter=1;                 //counts how many elements in the sorted array have the same value with the start element
    for(uint64_t i=start+1; i<Size; i++){
        if(rows && column[rows[Array[i]]]!=column[rows[Array[start]]]) break;
        if(!rows && column[Array[i]]!=column[Array[start]]) break;   //values are saved in array column
        counter++;
    }
    return counter;
}

array::array(uint64_t size, uint64_t *values, uint64_t *rs) {
    Size=size;
    Array=values;
    rows=rs;
}
