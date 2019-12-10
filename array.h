#ifndef ARRAY_H
#define ARRAY_H

#include <cstdint>
#include <iostream>

struct mytuple{
    uint64_t index;
    uint64_t value;
};

struct array {
    uint64_t Size;
    mytuple *Array;
    int c;
public:
    array(uint64_t size){
        Size=size;
        Array=new mytuple[Size];
        c=0;
    }
    void add(uint64_t v,uint64_t i){
        Array[c].value=v;
        Array[c].index=i;
        c++;
    }
    array(uint64_t size,uint64_t *values);
    ~array();
    uint64_t countKeys(uint64_t start);
};


#endif
