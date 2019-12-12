#ifndef ARRAY_H
#define ARRAY_H

#include <cstdint>
#include <iostream>


struct array {
    uint64_t Size;
    uint64_t *Array;
    int c;
public:
    array(uint64_t size){
        Size=size;
        Array=new uint64_t[Size];
        c=0;
    }
    void add(uint64_t v){
        Array[c]=v;
        c++;
    }
    array(uint64_t size,uint64_t *values);
    ~array();
    uint64_t countKeys(uint64_t start,uint64_t *column);
};


#endif
