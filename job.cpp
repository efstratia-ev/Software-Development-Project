#include "job.h"

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
    list *res;
    if(sorted) res=sortedjoin(array1,array2,column1,column2);
    else res=join(array1,array2,column1,column2);
    delete array1;
    delete array2;
    bool results_exist=(res->get_size()>0);
    query->update_results(res,arrayID1,arrayID2);
    query->DoQuery(results_exist);
}
