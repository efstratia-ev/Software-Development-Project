#include "job.h"
#include "sort.h"

int QueryJob::Run() {
    query->DoQuery(query->execute_filters());
}


SortJob::SortJob(sem_t *f, Query *q, radix *r, int o, int s) {
    finished=f;
    query=q;
    R=r;
    offset=o;
    size=s;
    Stack=new stack();
}

int SortJob::Run() {
    R->split(Stack,offset,size);
    radix *currentRadix;
    while(Stack->notEmpty()){
        currentRadix=Stack->pop();
        currentRadix->group();  //make histogram+prefixSum and reorder rows_array
        currentRadix->split(Stack);
        delete currentRadix;
    }
    delete Stack;
    sem_post(finished);
}

MergeJob::MergeJob(Query *q, rows_array *a1, rows_array *a2, bool s, uint64_t *c1, uint64_t *c2, int id1,
                   int id2) {
    query=q;
    array1=a1;
    array2=a2;
    sorted=s;
    arrayID1=id1;
    arrayID2=id2;
    column1=c1;
    column2=c2;
}

int MergeJob::Run() {
    join(arrayID1,arrayID2,array1,array2,column1,column2,query,sorted);
}

JoinJob::JoinJob(sem_t *sem,Query *query, rows_array *array1, rows_array *array2, bool sorted,  uint64_t offset1,uint64_t size1, uint64_t offset2,uint64_t size2,uint64_t res_counter) {
    this->sem=sem;
    this->query=query;
    this->array1=array1;
    this->array2=array2;
    this->sorted=sorted;
    this->offset1=offset1;
    this->size1=size1;
    this->offset2=offset2;
    this->size2=size2;
    this->res_counter=res_counter;
    this->next=NULL;
}

int JoinJob::Run() {
    int curr=0;
    if(sorted){
        for(uint64_t x=offset1; x<size1; x++){
            for(uint64_t y=offset2; y<size2; y++){
                query->update_array_element(x,array2->Array[y],res_counter+curr++);
            }
        }
    }
    else {
        for(uint64_t x=offset1; x<size1; x++){
            for(uint64_t y=offset2; y<size2; y++){
                query->update_array_element(array1->Array[x],array2->Array[y],res_counter+curr++);
            }
        }
    }
    sem_post(sem);
    if(next)
        next->Run();
}

bool JoinJob::add(JoinJob *job) {
    if(get_counter()>=MIN) return false;
    job->next=next;
    next=job;
    return true;
}

/*
int JoinJob::Run() {
    list *resultlist=new list();
    if(sorted) add_sortjoin_results(offset1,size1,offset2,size2,array2,resultlist);
    else add_join_results(offset1,size1,offset2,size2,array1,array2,resultlist);
    resultlist->restart_current();
    query->update_array(resultlist,res_counter);
    sem_post(sem);
}*/
