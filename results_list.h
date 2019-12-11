#ifndef RESULTS_LIST_H
#define RESULTS_LIST_H

#include "JoinArray.h"


class results_node{
    int size;
    uint64_t *sums;
    results_node *next;
public:
    results_node(int sz,uint64_t *s,results_node *n){
        size=sz;
        sums=s;
        next=n;
    }
    ~results_node() {
        delete sums;
        delete next;
    }
    void print(FILE *file){
        if(next) next->print(file);
        for(int i=0; i<size; i++){
            if(sums[i]!=0) {
                cout<<sums[i];
                fprintf(file,"%ld",sums[i]);
            }
            else{
                cout<<"NULL";
                fprintf(file,"NULL");
            }
            if(i!=size-1){
                cout<<" ";
                fprintf(file," ");
            }
            else{
                cout<<endl;
                fprintf(file,"\n");
            }
        }
    }
    results_node *get_next(){
        return next;
    }
};

class results_list {
    results_node *start;
    int counter;
public:
    results_list();
    ~results_list();
    void add(int size,uint64_t *sums){
        start=new results_node(size,sums,start);
        counter++;
    }
    void print(FILE *file){
        start->print(file);
    }
    void clear();
};


#endif
