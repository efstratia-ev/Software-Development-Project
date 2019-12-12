#ifndef ARRAY_H
#define ARRAY_H

#include <cstdint>
#include <iostream>

using namespace std;


struct mytuple{
    uint64_t index;
    uint64_t value;
};

struct array {
    uint64_t Size;
    uint64_t *Array;
public:
    array(uint64_t size,uint64_t *values);
    ~array();
    uint64_t countKeys(uint64_t start);
    void printValues() {
        cout << "array:" << endl;
        for (int i = 0; i < Size; i++) 
            cout << Array[i] << endl;
    }
    uint64_t countKeys(uint64_t start,const uint64_t *column);
};


#endif
