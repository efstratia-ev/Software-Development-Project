#ifndef SQLO
#define SQLO
#include "Priority_Queue.h"
#include "relations.h"

using namespace std;

class SQL{  //keeps all the information needed from the query
    int filters{};
    char * query;
    int * from_arrays{},from_arrays_sz{},select_result_sz{};
    Priority_Queue * where_predicates;
    set * select_results;
    //how many inner joins the query has
    int numInnerJoins;
    Relations *rels;
    int *joinOrder;
    uint64_t QueryMinCost;
    int initJoinStatsSz;
public:
    explicit SQL(char *line,Relations *rels);
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
    int get_from_arrays_size() { return from_arrays_sz; }
    int getNumInnerJoins() { return numInnerJoins; }
    int get_filters_num();
    int get_results_counter();
    set *get_select();
    int *get_from_arrays();
    void EvaluateMinCost();
    void  JoinsPermutations(int *indices,int size,uint64_t min);
    Relations *applyJoinStats(Relations *relsStats,Predicate * pr,uint64_t &cost );
    void print(){
        for(int i=0;i<numInnerJoins;i++) cout<<joinOrder[i] <<" ";
        cout<<endl;
    }
    bool CheckConnection(int *indices);



    };

#endif