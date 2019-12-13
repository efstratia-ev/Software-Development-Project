#include "Priority_Queue.h"

using namespace std;

class SQL{
    int filters{};
    char * query;
    int * from_arrays{},from_arrays_sz{},select_result_sz{};
    Priority_Queue * where_predicates;
    set * select_results{};
public:
    explicit SQL(char *line);
    ~SQL();
    int CutQueryToParts();
    void InitFromArrays(const string&);
    void GetFromArrays(const string&);
    void GetWherePredicates(const string&);
    void SplitWherePredicates(const string&);
    static bool isFilter(const string& predicate);
    void GetWhereFilters(string);
    void InitSelectResults(const string&);
    void SplitSelectResults(const string& select);
    void GetSelectResults(const string&,int);
    Predicate *getPredicate();
    int get_filters_num();
    int get_results_counter();
    set *get_select();
    int *get_from_arrays();
};



