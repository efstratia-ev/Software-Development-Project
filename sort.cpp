#include <iostream>
#include "sort.h"

using namespace std;

void join(int arr1,int arr2,rows_array *rows_array1,rows_array *rows_array2,uint64_t *column1,uint64_t *column2,Query *Q,bool sorted) {  //does the final comparison between two sorted rows_arrays
    queue *joins=new queue();
    uint64_t i=0,j=0;
    sem_t *sem=new sem_t;
    sem_init(sem, 0, 0);
    uint64_t count_results=0,count_jobs=0;
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
        joins->pushJoin(new JoinJob(sem,Q,rows_array1,rows_array2,sorted,i,maxi,j,maxj,count_results));
        count_jobs++;
        count_results+=(maxi-i)*(maxj-j);
        i=maxi;
        j=maxj;
    }
    Q->add_joined_array(count_results,arr1,arr2);
    if(joins->getSize()>0) {
        js->Schedule(joins);
        js->Schedule(new PredicateJob(Q, true,rows_array1,rows_array2), sem, count_jobs);
        return;
    }
    js->Schedule(new PredicateJob(Q, false,rows_array1,rows_array2));
    delete sem;
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

void add_join_results(uint64_t offset1, uint64_t size1, uint64_t offset2, uint64_t size2, rows_array *rows_array1,
                      rows_array *rows_array2, list *resultlist) {
    for(uint64_t x=offset1; x<size1; x++){
        for(uint64_t y=offset2; y<size2; y++){
            uint64_t a=rows_array1->Array[x],b=rows_array2->Array[y];
            resultlist->add(rows_array1->Array[x],b);
        }
    }
}

void add_sortjoin_results(uint64_t offset1, uint64_t size1, uint64_t offset2, uint64_t size2, rows_array *rows_array2,
                          list *resultlist) {
    for(uint64_t x=offset1; x<size1; x++){
        for(uint64_t y=offset2; y<size2; y++){
            resultlist->add(x,rows_array2->Array[y]);
        }
    }
}

list *join(rows_array *array1, rows_array *array2, uint64_t *column1, uint64_t *column2) { //counts how many arrays are already in join results
    list *resultlist = new list();
    uint64_t i = 0, j = 0;
    while (i < array1->Size && j < array2->Size) {
        if (column1[array1->get_value(i)] > column2[array2->get_value(j)]) {
            j += array2->countKeys(j, column2);
            continue;
        }
        if (column1[array1->get_value(i)] < column2[array2->get_value(j)]) {
            i += array1->countKeys(i, column1);
            continue;
        }
        uint64_t maxi = i + array1->countKeys(i, column1), maxj = j + array2->countKeys(j, column2);
        for (uint64_t x = i; x < maxi; x++) {
            for (uint64_t y = j; y < maxj; y++) {
                resultlist->add(array1->Array[x], array2->Array[y]);
            }
        }
        i = maxi;
        j = maxj;
    }
    resultlist->restart_current();
    return resultlist;
}

list *sortedjoin(rows_array *array1, rows_array *array2, uint64_t *column1, uint64_t *column2) { //counts how many arrays are already in join results
    list *resultlist = new list();
    uint64_t i = 0, j = 0;
    while (i < array1->Size && j < array2->Size) {
        if (column1[array1->get_value(i)] > column2[array2->get_value(j)]) {
            j += array2->countKeys(j, column2);
            continue;
        }
        if (column1[array1->get_value(i)] < column2[array2->get_value(j)]) {
            i += array1->countKeys(i, column1);
            continue;
        }
        uint64_t maxi = i + array1->countKeys(i, column1), maxj = j + array2->countKeys(j, column2);
        for (uint64_t x = i; x < maxi; x++) {
            for (uint64_t y = j; y < maxj; y++) {
                resultlist->add(x, array2->Array[y]);
            }
        }
        i = maxi;
        j = maxj;
    }
    resultlist->restart_current();
    return resultlist;
}


//sort from part2
rows_array *sort(radix *r) {  //function that sort rows_arrays by quantum
    auto *Stack=new stack();
    Stack->push(r);
    radix *currentRadix;
    while(Stack->notEmpty()){
        currentRadix=Stack->pop();
        currentRadix->group();  //make histogram+prefixSum and reorder rows_array
        currentRadix->split(Stack);
        if(currentRadix!=r) delete currentRadix;
    }
    delete Stack;
    rows_array *sortedR=r->getR();
    r->delete_R();
    delete r;
    return sortedR;
}