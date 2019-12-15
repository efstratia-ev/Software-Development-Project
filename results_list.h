#ifndef RESULTS_LIST_H
#define RESULTS_LIST_H

#include "JoinArray.h"


class results_node{
    int size;
    uint64_t *sums;
    results_node *next;
public:
    results_node(int sz,uint64_t *s,results_node *n);
    ~results_node();
    void print(FILE *file);
};

class results_list {  //keeps all the results from queries
    results_node *start;
    int counter;
public:
    results_list();
    ~results_list();
    void add(int size,uint64_t *sums);
    void print(FILE *file);
    void clear();
};


#endif
