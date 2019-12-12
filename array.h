#ifndef ARRAY_H
#define ARRAY_H

#include <cstdint>
#include <iostream>


struct array {
    uint64_t Size;
    uint64_t *Array;
public:
    array(uint64_t size,uint64_t *values);
    uint64_t countKeys(uint64_t start,const uint64_t *column);
};


#endif
