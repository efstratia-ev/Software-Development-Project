#ifndef STATISTICS
#define STATISTICS
#include <cstdint>
#include "bitmap.h"

class stats {
    uint64_t min;
    uint64_t max;
    uint64_t distinctValues;
    uint64_t totalValues;
    BitMap *bm;
    public:
    stats(uint64_t min,uint64_t max,BitMap *bm,uint64_t totalValues,uint64_t distinctValues);
    uint64_t getMin();
    uint64_t getMax();
    uint64_t getDistinctValues();
    uint64_t getTotalValues();
    BitMap *getBitMap();
    void setMin(uint64_t min);
    void setMax(uint64_t max);
    void setDistinctValues(uint64_t distinctValues);
    void setTotalValues(uint64_t totalValues);
    void delete_bit_map();
    ~stats();

};

#endif