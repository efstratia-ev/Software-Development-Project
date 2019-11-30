#ifndef RESULTS_LIST_H
#define RESULTS_LIST_H

#include "ResultsArray.h"


class results_node{
    ResultsArray *results;
    results_node *next;
public:
    results_node(ResultsArray *a,results_node *n){
        results=a;
        next=n;
    }
    ~results_node(){
        delete next;
    }
};

class results_list {
    results_node *start;
    int counter;
public:
    results_list(){
        counter=0;
        start=NULL;
    }
    ~results_list(){
        delete start;
    }
    void add(ResultsArray *results){
        start=new results_node(results,start);
        counter++;
    }
    void clear(){
        delete start;
        start=NULL;
        counter=0;
    }
};


#endif
