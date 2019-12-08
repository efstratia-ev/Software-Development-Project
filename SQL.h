#define COLUMNSPRE 4
#define COLUMNSRE 2

#include <iostream>

class set{
public:
    int array;
    int column;

    set();

    set(int array, int column);
};

class filter{
public:
    char comparison;
    int number;
    int array;
    int column;
    filter * next;

};

class list_filters{
    int size;
    filter * head;
public:
    list_filters();
    void Push(char comparison,int filter_v,int a,int c);
    void Pop();
    bool IsEmpty();

    int getSize() const;
};

class node{
public:
    int array1;
    int column1;
    int array2;
    int column2;
    node * next;
};

class Priority_Queue{
    int size;
    node * head;

public:
    Priority_Queue();
    void Push(int a1,int c1,int a2,int c2);
    void Pop();
    //void Rearrange(int,int);
    bool IsEmpty();
};

using namespace std;

class SQL{
    char * query;
    int filters;
    int current;
    int * from_arrays,from_arrays_sz,select_result_sz;
    list_filters * where_filters;
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
    int get_next_filter();
    int *get_next_join();
    int get_filters_counter();
};



