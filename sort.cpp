#include <iostream>
#include "sort.h"
using namespace std;
list *join(array *array1,array *array2,uint64_t *column1,uint64_t *column2,int count_arrays) { //counts how many arrays are already in join results
    list *resultlist=new list();
    uint64_t i=0,j=0;
    while(i<array1->Size && j<array2->Size){
        if(column1[array1->Array[i]]>column2[array2->Array[j]]){
            j+=array2->countKeys(j,column2);
            continue;
        }
        if(column1[array1->Array[i]]<column2[array2->Array[j]]){
            i+=array1->countKeys(i,column1);
            continue;
        }
        uint64_t maxi=i+array1->countKeys(i,column1),maxj=j+array2->countKeys(j,column2);
        for(uint64_t x=i; x<maxi; x++){
            for(uint64_t y=j; y<maxj; y++){
                resultlist->add(array1->Array[x],array2->Array[y]);
                /*if(count_arrays==0) resultlist->add(array1->Array[x],array2->Array[y]);
                else if(count_arrays==1) resultlist->add(x,array2->Array[y]);
                else resultlist->add(x,y);*/
            }
        }
        i=maxi;
        j=maxj;
    }
    resultlist->restart_current();
    return resultlist;
}

array *sort(radix *r) {
    auto *Stack=new stack();
    Stack->push(r);
    radix *currentRadix;
    while(Stack->notEmpty()){
        currentRadix=Stack->pop();
        currentRadix->group();  //make histogram+prefixSum and reorder array
        currentRadix->split(Stack);
        if(currentRadix!=r) delete currentRadix;
    }
    delete Stack;
    array *sortedR=r->getR();
    r->delete_R();
    delete r;
    return sortedR;
}