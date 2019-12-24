#ifndef STATS
#define STATS
#include <cstdint>

class stats {
    uint64_t min;
    uint64_t max;
    uint64_t distinctValues;
    public:
    stats(uint64_t min,uint64_t max, uint64_t distinctValues) {
        this->min = min;
        this->max = max;
        this->distinctValues = distinctValues;
    }
    uint64_t getMin() { return min; }
    uint64_t getMax() { return max; }
    uint64_t getUniqueValues() { return distinctValues; }
};

#endif