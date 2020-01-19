#ifndef QUERY_H
#define QUERY_H

#include "relations.h"
#include "SQL.h"
#include "JoinArray.h"

enum jointype_t {create=0,update=1,update_filtered=2};

//options for parallelism.
//if any of these fields is set true, then we'll 
//try to execute it in parallel.
struct ParallelismOpts {
    //At the moment we support only both of them or none of them.
    //So they'll be both set true or both set false.
    bool join; 
    bool sort; 
};

class Query {
    Relations *relations;
    SQL *sql;
    uint64_t *sums;
    JoinArray *results, **filter_results;
    int max;
    jointype_t type;
    ParallelismOpts opts;
public:
    Query(Relations *rels,SQL* s,uint64_t *sm,struct ParallelismOpts opts);
    ~Query();
    int isRelationFiltered(int relation);
    bool execute_filter(Predicate *predicate);
    bool execute_filters();
    bool DoQuery(bool filters);
    bool RunQuery(bool filters);
    bool RunQueryWithoutParallelism(bool filters);
    void add_joined_array(uint64_t size,int array1,int array2);
    void update_array(list *res,uint64_t offset);
    void update_array_element(uint64_t num1,uint64_t num2,uint64_t i);
    jointype_t get_type(){
        return type;
    }
    SQL *getSQL() { return sql; }
    uint64_t *getSums() { return sums; }
};


#endif
