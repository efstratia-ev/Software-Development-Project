#define COLUMNSPRE 4
#define COLUMNSRE 2

#include <iostream>

using namespace std;

class set{
    int array;
    int column;
public:
    set();
    set(int array, int column);

    int getArray() const;

    int getColumn() const;

    void setArray(int array);

    void setColumn(int column);
};

class Predicate{
    set *array1;
public:
    Predicate(int a,int c){
        array1=new set(a,c);
    }
};

class comparison:public Predicate{
    char comp;
    int num;
public:
    comparison(int a,int c,char cmp,int n):Predicate(a,c){
        comp=cmp;
        num=n;
    }
};

class join:public Predicate{
    set *array2;
public:
    join(int a1,int c1,int a2,int c2):Predicate(a1,c1){
        array2=new set(a2,c2);
    }
};



class node{
    Predicate *predicate;
    node * next;
public:
    node(Predicate *p,node *n){
        predicate=p;
        next=n;
    }
    node* get_next(){
        return next;
    }
};

class Priority_Queue{
    int size;
    node * head;

public:
    Priority_Queue();
    void Push(Predicate *p);
    Predicate *Pop();
    //void Rearrange(int,int);
    bool IsEmpty();
};


class SQL{
    char * query;
    int filters;
    int current;
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



