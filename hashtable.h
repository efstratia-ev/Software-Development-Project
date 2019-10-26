#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <cinttypes>
#include "list.h"
#include "stack.h"
#define N 256

class stack;

struct mytuple{
    int64_t index;
    int64_t value;
};

class hashtable{
    int64_t offset;
    int64_t size;
    mytuple *R;
    int64_t Hist[N]={0};
    int64_t Psum[N]={0};
    mytuple *_R;
    int64_t byte;
public:
    hashtable(int64_t o,int64_t s,mytuple *r,mytuple *_r,int64_t b);
    void initializeHist();
    void initializePsum();
    void reorderR();
    int64_t hash1(int64_t value);
    bool fit_cache(int64_t i);
    void split(stack *Stack);
    void quickshort(int64_t start,int64_t end);
    int64_t partition(int64_t start,int64_t end);
    void print();
};

#endif