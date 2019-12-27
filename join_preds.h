#ifndef I_AM_BORED
#define I_AM_BORED
#include "radix.h"
#include "stack.h"
//#include "sort.h"
//#include "array.h"
#include "relations.h"
#include "results_list.h"
#include "JoinArray.h"
#include "SQL.h"


JoinArray *searchFiltered(int relation,int max,JoinArray **filtered);
JoinArray *joinFirstPredicate(JoinArray **filtered,SQL *sql,Relations *rels,int max);
JoinArray *joinPredicates(JoinArray **filtered,SQL *sql,Relations *rels,int max);
//uint64_t *DoQuery(SQL *sql, Relations *relations);
int isRelationFiltered(JoinArray **filtered, int max, int relation);


#endif