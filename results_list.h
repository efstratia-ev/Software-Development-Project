#ifndef RESULTS_LIST_H
#define RESULTS_LIST_H

#include "JoinArray.h"


class results_node{
    JoinArray *results;
    results_node *next;
public:
    results_node(JoinArray *a,results_node *n);
    ~results_node();
};

class results_list {
    results_node *start;
    int counter;
public:
    results_list();
    ~results_list();
    void add(JoinArray *results);
    void clear();
};


#endif
