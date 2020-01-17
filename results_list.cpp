#include <iostream>
#include "results_list.h"

using namespace std;

results_list::results_list() {
    counter=0;
    start=nullptr;
}

results_list::~results_list() {
    delete start;
}


void results_list::clear() {
    delete start;
    start=nullptr;
    counter=0;
}

void results_list::add(int size, uint64_t *sums) {
    start= new results_node(size, sums, start);
    counter++;
}

void results_list::print(FILE *file) {
    start->print(file);
}

void results_list::print() {
    start->print();
}

results_node::results_node(int sz, uint64_t *s, results_node *n){
    size=sz;
    sums=s;
    next=n;
}

results_node::~results_node() {
    delete[] sums;
    delete next;
}

void results_node::print(FILE *file) {
    if(next) next->print(file);
    if(!sums){
        for(int i=0; i<size; i++){
            if(i==size-1){
                cout<<"NULL"<<endl;
                fprintf(file,"NULL\n");
            }
            else{
                cout<<"NULL"<<" ";
                fprintf(file,"NULL ");
            }
        }
        return;
    }
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

void results_node::print() {
    if(next) next->print();
    if(!sums){
        for(int i=0; i<size; i++){
            if(i==size-1){
                cout<<"NULL"<<endl;
            }
            else{
                cout<<"NULL"<<" ";
            }
        }
        return;
    }
    for(int i=0; i<size; i++){
        if(sums[i]!=0) {
            cout<<sums[i];
        }
        else{
            cout<<"NULL";
        }
        if(i!=size-1){
            cout<<" ";
        }
        else{
            cout<<endl;
        }
    }
}