#ifndef JOB
#define JOB

#include <semaphore.h>
#include "relations.h"
#include "SQL.h"
#include "query.h"
#include "stack.h"

list *join(rows_array *rows_array1,rows_array *rows_array2,uint64_t *column1,uint64_t *column2);

list *sortedjoin(rows_array *rows_array1,rows_array *rows_array2,uint64_t *column1,uint64_t *column2);

class radix;
class sorted_radix;

//#include "do_query.h"


class Job { 
    public:
    virtual int Run() = 0;
   // ~Job() {};
};

class QueryJob : public Job {
    Query *query;
    public:
    QueryJob(SQL *sql, Relations *rels, uint64_t *&sums) {
        query=new Query(rels,sql,sums);
    }
    int Run();
};

class SortJob:public Job{
    sem_t *finished;
    Query *query;
    radix *R;
    int offset;
    int size;
    stack *Stack;
public:
    SortJob(sem_t *f,Query *q,radix *r,int o,int s);
    int Run();
};

class MergeJob:public Job{
    Query *query;
    rows_array *array1;
    rows_array *array2;
    int arrayID1;
    int arrayID2;
    uint64_t *column1;
    uint64_t *column2;
    bool sorted;
public:
    MergeJob(Query *q,rows_array *a1,rows_array *a2,bool s,uint64_t *c1,uint64_t *c2,int id1,int id2);
    int Run();
};

class JoinJob:public Job{
    sem_t *sem;
    Query *query;
    rows_array *array1;
    rows_array *array2;
    bool sorted;
    uint64_t offset1;
    uint64_t size1;
    uint64_t offset2;
    uint64_t size2;
    uint64_t res_counter;
public:
    JoinJob(sem_t *sem,Query *query, rows_array *array1, rows_array *array2, bool sorted, uint64_t offset1,uint64_t size1, uint64_t offset2,uint64_t size2,uint64_t res_counter);
    int Run();
};

class PredicateJob:public Job{
    Query *query;
    bool results_exist;
public:
    PredicateJob(Query *query,bool results_exist){
        this->query=query;
        this->results_exist=results_exist;
    }
    int Run(){
        query->DoQuery(results_exist);
    }
};
#endif