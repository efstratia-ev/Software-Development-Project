#ifndef STATS
#define STATS
#include <cstdint>
#include "bitmap.h"

class stats {
    uint64_t min;
    uint64_t max;
    uint64_t distinctValues;
    uint64_t totalValues;
    BitMap *bm;
    public:
    stats(uint64_t min,uint64_t max,BitMap *bm,uint64_t totalValues,uint64_t distinctValues) {
        this->min = min;
        this->max = max;
        this->bm = bm;
        this->distinctValues = distinctValues;
        this->totalValues = totalValues;
    }
    uint64_t getMin() { return min; }
    uint64_t getMax() { return max; }
    uint64_t getDistinctValues() { return distinctValues; }
    uint64_t getTotalValues() { return totalValues; }
    BitMap *getBitMap() { return bm; }
    void setMin(uint64_t min) { this-> min = min; }
    void setMax(uint64_t max) { this-> max = max; }
    void setDistinctValues(uint64_t distinctValues) { this->distinctValues = distinctValues; }
    void setTotalValues(uint64_t totalValues) { this->totalValues = totalValues; }
    void print() {
        cout << min << "," << max << "," << distinctValues << "," << totalValues << endl;
    }
    void delete_bit_map(){
        delete bm;
    }
    ~stats(){}

};

#endif