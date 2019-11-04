#include <iostream>
#include "sort.h"
using namespace std;
list *finalresults(mytuple *array1, mytuple *array2, uint64_t size1, uint64_t size2) {
    list *resultlist=new list();
    uint64_t start=0,j;
    for(uint64_t i=0; i<size1; i++){
        for(j=start; j<size2; j++){
            if(array1[i].value==array2[j].value) resultlist->add(array1[i].index,array2[j].index);
            else if(array1[i].value<array2[j].value)
                break;
        }
        if(i!=size1-1 && array1[i].value!=array1[i+1].value)  start=j;
    }
    return resultlist;
}

void sort(radix *r) {
    stack *Stack=new stack();
    Stack->push(r);
    radix *currentRadix;
    while(Stack->notEmpty()){
        currentRadix=Stack->pop();
        currentRadix->group();  //make histogram+prefixSum and reorder array
        currentRadix->split(Stack);
        if(currentRadix!=r) delete currentRadix;
    }
    delete Stack;

}

int countResults(mytuple *array1, mytuple *array2, uint64_t size1, uint64_t size2) {
    uint64_t start=0,j;
    int count=0;
    for(uint64_t i=0; i<size1; i++){
        for(j=start; j<size2; j++){
            if(array1[i].value==array2[j].value) count++;
            else if(array1[i].value<array2[j].value)
                break;
        }
        if(i!=size1-1 && array1[i].value!=array1[i+1].value)  start=j;
    }
    return count;
}