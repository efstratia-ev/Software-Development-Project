#ifndef ARRAY_H
#define ARRAY_H

#include <cstdint>
#include <iostream>

struct array {
    uint64_t Size;
    uint64_t *Array;
    uint64_t *rows;
public:
    array(uint64_t size,uint64_t *values);
    array(uint64_t size,uint64_t *values,uint64_t *rs);
    uint64_t countKeys(uint64_t start,const uint64_t *column);
    uint64_t get_value(uint64_t row){
        if(rows) return rows[Array[row]];
        else return Array[row];
    }
};


#endif
