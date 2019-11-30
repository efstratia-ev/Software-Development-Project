#include "array.h"

array::array(uint64_t size, uint64_t *values) {
    Size=size;
    Array=new mytuple[size];
    for(int i=0; i<size; i++){
        Array[i].index=i;
        Array[i].value=values[i];
    }
}

array::~array() {
    delete[] Array;
}

uint64_t array::countKeys(uint64_t start) {
    uint64_t counter=1;
    for(uint64_t i=start+1; i<Size; i++){
        if(Array[i].value!=Array[start].value) break;
        counter++;
    }
    return counter;
}
