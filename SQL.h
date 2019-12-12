#include "Priority_Queue.h"

#define COLUMNSPRE 4
#define COLUMNSRE 2

using namespace std;

class SQL{
    int filters;
    char * query;
    int * from_arrays,from_arrays_sz,select_result_sz;
    Priority_Queue * where_predicates;
    set * select_results;
public:
    SQL(char *line);
    ~SQL();
    int CutQueryToParts();
    void InitFromArrays(string);
    void GetFromArrays(string);
    void GetWherePredicates(string);
    void SplitWherePredicates(string);
    bool isFilter(string predicate);
    void GetWhereFilters(string);
    void InitSelectResults(string);
    void SplitSelectResults(string select);
    void GetSelectResults(string,int);
    Predicate *getPredicate();
    int get_filters_num();
    int get_results_counter();
    set *get_select();
    bool same_array(int a,int b);
    int *get_from_arrays();
};



