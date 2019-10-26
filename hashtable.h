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
    int byte;
public:
    hashtable(int64_t o,int64_t s,mytuple *r,mytuple *_r,int b);
    void initializeHist();
    void initializePsum();
    void reorderR();
    int64_t hash1(int64_t value);
    bool fit_cache(int i);
    void split(stack *Stack);
};

#endif