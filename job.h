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


class JoinJob;

class Job {
    public:
    virtual void Run() = 0;
    virtual bool add(JoinJob *job)=0;
    virtual ~Job() {};
};

class QueryJob : public Job {
    Query *query;
    public:
    QueryJob(Query *query);
    void Run();
    bool add(JoinJob *job);
    ~QueryJob(){}
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
    void Run();
    bool add(JoinJob *job);
    ~SortJob();
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
    radix *R1;
    radix *R2;
public:
    MergeJob(Query *q,rows_array *a1,rows_array *a2,bool s,uint64_t *c1,uint64_t *c2,int id1,int id2,radix *R1,radix *R2);
    void Run();
    bool add(JoinJob *job);
    ~MergeJob() override;
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
    JoinJob *next;
public:
    JoinJob(sem_t *sem,Query *query, rows_array *array1, rows_array *array2, bool sorted, uint64_t offset1,uint64_t size1, uint64_t offset2,uint64_t size2,uint64_t res_counter);
    void Run();
    bool add(JoinJob *job);
    ~JoinJob();
    uint64_t get_counter();
};

class PredicateJob:public Job{
    Query *query;
    bool results_exist;
    rows_array *array1;
    rows_array *array2;
public:
    PredicateJob(Query *query,bool results_exist,rows_array *array1,rows_array *array2);
    void Run();
    bool add(JoinJob *job);
    ~PredicateJob() override;
};
#endif