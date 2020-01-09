#ifndef JOB
#define JOB
#include "relations.h"
#include "SQL.h"
#include "query.h"

class radix;
class sorted_radix;
class JobScheduler;

//#include "do_query.h"
extern JobScheduler *js;

class Job { 
    public:
    virtual int Run() = 0;
   // ~Job() {};
};

class QueryJob : public Job {
    Query *query;
    uint64_t *&s;
    public:
    QueryJob(SQL *sql, Relations *rels, uint64_t *&sums) : s(sums) {
        query=new Query(rels,sql);
    }
    int Run();
};

class SortJob:public Job{
    radix *Radix;
public:

};

#endif