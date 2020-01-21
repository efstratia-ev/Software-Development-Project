#include "stats.h"

stats::stats(uint64_t min, uint64_t max, BitMap *bm, uint64_t totalValues, uint64_t distinctValues) {
    this->min = min;
    this->max = max;
    this->bm = bm;
    this->distinctValues = distinctValues;
    this->totalValues = totalValues;
}

uint64_t stats::getMin() { return min; }

uint64_t stats::getMax() { return max; }

uint64_t stats::getDistinctValues() { return distinctValues; }

uint64_t stats::getTotalValues() { return totalValues; }

BitMap *stats::getBitMap() { return bm; }

void stats::setMin(uint64_t min) { this-> min = min; }

void stats::setMax(uint64_t max) { this-> max = max; }

void stats::setDistinctValues(uint64_t distinctValues) { this->distinctValues = distinctValues; }

void stats::setTotalValues(uint64_t totalValues) { this->totalValues = totalValues; }

void stats::delete_bit_map() {
    delete bm;
}

stats::~stats() {}
