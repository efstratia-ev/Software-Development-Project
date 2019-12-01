#define COLUMNSPRE 4
#define COLUMNSRE 2

#include <iostream>

using namespace std;

class SQLInfo{
    char *line;
    int filters;
    int current;
    int *Arrays,arraySize;
    int **Predicates,Predicates_sz;
    int **ArraysResults,Arrayresults_sz;
public:
    SQLInfo(char *l);
    ~SQLInfo();
    int getFROM();
    int findArrays(int pos);
    int getWHERE(int start);
    void makePredicates();
    void findPredicates(int pos);
    void getSELECT(int start);
    void findArraysResults(int pos);
    void makeArraysResults();
    int *get_next_filter();
    int *get_next_join();
    int get_filters_counter();
};



