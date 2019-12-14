#ifndef RADIX_H
#define RADIX_H

#include "array.h"
#include <cinttypes>
#include "list.h"
#include "stack.h"
#include "relation.h"
#define N 256
#define L1size 64000

class stack;

class radix{
    uint64_t offset;
    uint64_t size;
    uint64_t *R;
    uint32_t Hist[N]{};
    uint32_t Psum[N]{};
    uint64_t *_R;
    int byte;
    uint64_t *rows;
    uint64_t *data;
public:
    radix(uint64_t offset,uint64_t size, uint64_t *r,uint64_t *_r,uint64_t *d,uint64_t *rs,int byte = 1);
    radix(uint64_t size,uint64_t *r,uint64_t *d);
    radix(uint64_t size,uint64_t *d);
    void group();
    void histogram();
    void histogram_init();
    void prefixSum();
    void reorderR();
    uint64_t hash(uint64_t value);
    bool fitsCache(uint64_t i);
    void split(stack *Stack);
    void quicksort(uint64_t start,uint64_t end);
    uint64_t partition(uint64_t start,uint64_t end);
    array *getR();
    void delete_R();
};

#endif