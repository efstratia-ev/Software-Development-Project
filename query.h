#ifndef QUERY_H
#define QUERY_H

#include "relations.h"
#include "SQL.h"
#include "JoinArray.h"


class Query {
    Relations *relations;
    SQL *sql;
    uint64_t *&sums;
    JoinArray *results, **filter_results;
    int max;
public:
    Query(Relations *rels,SQL* s,uint64_t *&sm);
    int isRelationFiltered(int relation);
    bool execute_filter(Predicate *predicate);
    bool execute_filters();
    void DoQuery(bool filters);
    void update_results(list *res,int array1,int array2);
};


#endif
