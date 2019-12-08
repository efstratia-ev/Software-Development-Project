#ifndef RADIX_H
#define RADIX_H

#include "array.h"
#include <cinttypes>
#include "list.h"
#include "stack.h"
#define N 256
#define L1size 64000

class stack;


class radix{
    uint64_t offset;
    uint64_t size;
    mytuple *R;
    uint32_t Hist[N];
    uint32_t Psum[N];
    mytuple *_R;
    int byte;
public:
    radix(uint64_t offset,uint64_t size, mytuple *r,mytuple *_r,int byte = 1);
    radix(uint64_t size,mytuple *r);
    void group();
    void histogram();
    void prefixSum();
    void reorderR();
    uint64_t hash(uint64_t value);
    bool fitsCache(uint64_t i);
    void split(stack *Stack);
    void quicksort(int start,int end);
    int partition(int start,int end);
    void print();
};

#endif