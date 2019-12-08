#include "SQLInfo.h"

#define COLUMNSPRE 4
#define COLUMNSRE 2

using namespace std;

class SQL{
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

};



