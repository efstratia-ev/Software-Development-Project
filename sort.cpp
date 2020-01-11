#include <iostream>
#include "sort.h"

using namespace std;

list *join(rows_array *rows_array1,rows_array *rows_array2,uint64_t *column1,uint64_t *column2) {  //does the final comparison between two sorted rows_arrays
    list *resultlist=new list();
    uint64_t i=0,j=0;
    while(i<rows_array1->Size && j<rows_array2->Size){
        if(column1[rows_array1->get_value(i)]>column2[rows_array2->get_value(j)]){
            j+=rows_array2->countKeys(j,column2);
            continue;
        }
        if(column1[rows_array1->get_value(i)]<column2[rows_array2->get_value(j)]){
            i+=rows_array1->countKeys(i,column1);
            continue;
        }
        uint64_t maxi=i+rows_array1->countKeys(i,column1),maxj=j+rows_array2->countKeys(j,column2);
        for(uint64_t x=i; x<maxi; x++){
            for(uint64_t y=j; y<maxj; y++){
                resultlist->add(rows_array1->Array[x],rows_array2->Array[y]);
            }
        }
        i=maxi;
        j=maxj;
    }
    resultlist->restart_current();
    return resultlist;
}

rows_array *sort(sem_t *semaphore,Query *Q,radix *r) {  //function that sort rows_arrays by quantum
    r->group();
    int count = 0;
    for(int i=0; i<NUMJOBS; i++) {
        //js->Schedule(new SortJob(semaphore, Q, r, N / NUMJOBS * i, N / NUMJOBS * (i + 1)));
        js->Schedule(new SortJob(semaphore, Q, r, count, count+N/NUMJOBS));
        count += N /NUMJOBS;
    }
    return r->getR();
}

list *sortedjoin(rows_array *rows_array1, rows_array *rows_array2, uint64_t *column1, uint64_t *column2) {
    list *resultlist=new list();
    uint64_t i=0,j=0;
    while(i<rows_array1->Size && j<rows_array2->Size){
        if(column1[rows_array1->get_value(i)]>column2[rows_array2->get_value(j)]){
            j+=rows_array2->countKeys(j,column2);
            continue;
        }
        if(column1[rows_array1->get_value(i)]<column2[rows_array2->get_value(j)]){
            i+=rows_array1->countKeys(i,column1);
            continue;
        }
        uint64_t maxi=i+rows_array1->countKeys(i,column1),maxj=j+rows_array2->countKeys(j,column2);
        for(uint64_t x=i; x<maxi; x++){
            for(uint64_t y=j; y<maxj; y++){
                resultlist->add(x,rows_array2->Array[y]);
            }
        }
        i=maxi;
        j=maxj;
    }
    resultlist->restart_current();
    return resultlist;
}
