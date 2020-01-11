#ifndef RADIX_H
#define RADIX_H

#include "rows_array.h"
#include "list.h"
#include "relation.h"
#include "stack.h"

#define N 512
#define L1size 640

class stack;

class radix{
protected:
    uint64_t offset;
    uint64_t size;
    uint64_t *R;
    uint32_t Hist[N]{};
    uint32_t Psum[N]{};
    uint64_t *_R;
    int byte;
    uint64_t *data;
public:
    radix(uint64_t offset,uint64_t size, uint64_t *r,uint64_t *_r,uint64_t *d,int byte = 1);
    radix(uint64_t size,uint64_t *r,uint64_t *d);
    radix(uint64_t size,uint64_t *d);
    void group();
    virtual void histogram();
    virtual void histogram_init();
    void prefixSum();
    virtual void reorderR();
    uint64_t hash(uint64_t value);
    bool fitsCache(uint64_t i);
    virtual void split(stack *Stack);
    virtual void split(stack *Stack,int offset,int size);
    void quicksort(uint64_t start,uint64_t end);
    virtual uint64_t partition(uint64_t start,uint64_t end);
    virtual rows_array *getR();
    void delete_R();
};

class sorted_radix:public radix{
    uint64_t *rows;
public:
    sorted_radix(uint64_t offset,uint64_t s, uint64_t *r,uint64_t *_r,uint64_t *d,uint64_t *rs,int b);
    sorted_radix(uint64_t s,uint64_t *r,uint64_t *d);
    void histogram();
    void histogram_init();
    void reorderR();
    uint64_t partition(uint64_t start,uint64_t end);
    void split(stack *Stack);
    rows_array *getR();
    void split(stack *Stack,int offset,int size);
};
#endif