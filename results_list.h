#ifndef RESULTS_LIST_H
#define RESULTS_LIST_H

#include "ResultsArray.h"


class results_node{
    ResultsArray *results;
    results_node *next;
public:
    results_node(ResultsArray *a,results_node *n);
    ~results_node();
};

class results_list {
    results_node *start;
    int counter;
public:
    results_list();
    ~results_list();
    void add(ResultsArray *results);
    void clear();
};


#endif
