#include "SQLInfo.h"

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
    int CutQueryToParts();
    int InitFromArrays(string);
    void GetFromArrays(string);
    void GetWherePredicates(string);
    void SplitWherePredicates(string);
    bool isFilter(string predicate);
    void GetWhereFilters(string);
    int InitSelectResults(string);
    void SplitSelectResults(string select);
    void GetSelectResults(string,int);
    Predicate *getPredicate();
    int get_filters_num(){
        return filters;
    }
    int get_results_counter(){
        return select_result_sz;
    }
    set *get_select(){
        return select_results;
    }
};



