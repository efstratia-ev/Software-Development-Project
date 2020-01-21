#include "job.h"
#include "sort.h"
#include "select_options.h"

void QueryJob::Run() {
    if(query->DoQuery(query->execute_filters())) delete query;
}

QueryJob::QueryJob(Query *query) {
    this->query=query;
}

bool QueryJob::add(JoinJob *job) {
    return false;
}


SortJob::SortJob(sem_t *f, Query *q, radix *r, int o, int s) {
    finished=f;
    query=q;
    R=r;
    offset=o;
    size=s;
    Stack=new stack();
}

void SortJob::Run() {
    R->split(Stack,offset,size);
    radix *currentRadix;
    while(Stack->notEmpty()){
        currentRadix=Stack->pop();
        currentRadix->group();  //make histogram+prefixSum and reorder rows_array
        currentRadix->split(Stack);
        delete currentRadix;
    }
    sem_post(finished);
}

bool SortJob::add(JoinJob *job) {
    return false;
}

SortJob::~SortJob() {
    delete Stack;
}

MergeJob::MergeJob(Query *q, rows_array *a1, rows_array *a2, bool s, uint64_t *c1, uint64_t *c2, int id1,
                   int id2,radix *R1,radix *R2) {
    query=q;
    array1=a1;
    array2=a2;
    sorted=s;
    arrayID1=id1;
    arrayID2=id2;
    column1=c1;
    column2=c2;
    this->R1=R1;
    this->R2=R2;
}

void MergeJob::Run() {
    join(arrayID1,arrayID2,array1,array2,column1,column2,query,sorted);
}

bool MergeJob::add(JoinJob *job) {
    return false;
}

MergeJob::~MergeJob() {
    if(R1) R1->delete_R();
    if(R2) R2->delete_R();
    delete R1;
    delete R2;
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

void JoinJob::Run() {
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
    if(next){
        next->Run();
    }
}

bool JoinJob::add(JoinJob *job) {
    if(get_counter()>=MIN_JOIN_GROUP_ITEMS) return false;
    job->next=next;
    next=job;
    return true;
}

JoinJob::~JoinJob() {
    delete next;
}

uint64_t JoinJob::get_counter() {
    if(next) return next->get_counter()+(size1-offset1)*(size2-offset2);
    else return (size1-offset1)*(size2-offset2);
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
PredicateJob::PredicateJob(Query *query, bool results_exist, rows_array *array1, rows_array *array2) {
    this->array1=array1;
    this->array2=array2;
    this->query=query;
    this->results_exist=results_exist;
}

void PredicateJob::Run() {
    if(query->DoQuery(results_exist)) delete query;
}

bool PredicateJob::add(JoinJob *job) {
    return false;
}

PredicateJob::~PredicateJob() {
    delete array1;
    delete array2;
}
