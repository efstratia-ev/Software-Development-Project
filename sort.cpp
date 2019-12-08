#include <iostream>
#include "sort.h"
using namespace std;
list *join(array *array1,array *array2) {
    list *resultlist=new list();
    uint64_t i=0,j=0;
    while(i<array1->Size && j<array2->Size){
        if(array1->Array[i].value>array2->Array[j].value){
            j+=array2->countKeys(j);
            continue;
        }
        if(array1->Array[i].value<array2->Array[j].value){
            i+=array1->countKeys(i);
            continue;
        }
        uint64_t maxi=i+array1->countKeys(i),maxj=j+array2->countKeys(j);
        for(uint64_t x=i; x<maxi; x++){
            for(uint64_t y=j; y<maxj; y++){
                resultlist->add(array1->Array[x].value,array2->Array[y].value);
            }
        }
        i+=maxi;
        j=+maxj;
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