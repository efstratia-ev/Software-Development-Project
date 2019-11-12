#ifndef ARRAY_H
#define ARRAY_H

#include <cstdint>
#include <iostream>

struct mytuple{
    uint64_t index;
    uint64_t key;
    uint64_t value;
};

struct array {
    uint64_t Size;
    mytuple *Array;
public:
    array(uint64_t size){
        Size=size;
        Array=new mytuple[size];
    }
    ~array(){
        delete[] Array;
    }
    uint64_t countKeys(uint64_t start){
        uint64_t counter=1;
        for(uint64_t i=start+1; i<Size; i++){
            if(Array[i].key!=Array[start].key) break;
            counter++;
        }
        return counter;
    }
};


#endif
