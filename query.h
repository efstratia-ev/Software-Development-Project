#ifndef QUERY_H
#define QUERY_H

#include "relations.h"
#include "SQL.h"
#include "JoinArray.h"

enum jointype_t {create=0,update=1,update_filtered=2};

class Query {
    Relations *relations;
    SQL *sql;
    uint64_t *sums;
    JoinArray *results, **filter_results;
    int max;
    jointype_t type;
public:
    Query(Relations *rels,SQL* s,uint64_t *sm);
    ~Query();
    int isRelationFiltered(int relation);
    bool execute_filter(Predicate *predicate);
    bool execute_filters();
    bool DoQuery(bool filters);
    void add_joined_array(uint64_t size,int array1,int array2);
    void update_array(list *res,uint64_t offset);
    void update_array_element(uint64_t num1,uint64_t num2,uint64_t i);
    jointype_t get_type(){
        return type;
    }
};


#endif
