#ifndef JOB
#define JOB
#include "join_preds.h"

class Job { 
    public:
    virtual int Run() = 0;
    //virtual ~Job() {};
};

class QueryJob : public Job {
    SQL *sql;
    Relations *rels;
    uint64_t **sums;
    public:
    QueryJob(SQL *sql,Relations *rels,uint64_t **sums) {
        this->sql = sql;
        this->rels = rels;
        this->sums = sums;
    }
    int Run() {
        *sums = join(sql,rels);
    }
};

#endif