#ifndef PROJECT2019_RESULTSARRAY_H
#define PROJECT2019_RESULTSARRAY_H


#include "list.h"
#include "relations.h"


class Query;

class JoinArray {
    Relations *rels; //Join needs this
    uint64_t size{}; //size of joinResults
    uint64_t **Array{};
    int numRels; //how many rels are in JoinArray
    int *relationIDs{}; //which rels
    //relation belonging to relationIDs array that we 
    //will apply the next join on.
    int relToBeJoined{};
public:
    void setrel(int ar);
    int getNumRels();
    int getSize();
    uint64_t **getArray();
    explicit JoinArray(Relations *r);
    ~JoinArray();
    uint64_t get_value(uint64_t i);
    void update_array(list *results,int id);
    void update_array(list *results,JoinArray *array2);
    void create_array(list *results,int id);
    void create_array(list *results,int id1,int id2);
    int get_column(int arrayID);
    void filter_update(list *results);
    void compare(int arrayID,uint64_t column1,uint64_t column2);
    void compare(int arrayID1,uint64_t column1,int arrayID2,uint64_t column2);
    void grater_than(uint64_t column,uint64_t value);
    void less_than(uint64_t column,uint64_t value);
    void equal(uint64_t column,uint64_t value);
    bool exists(int arrayID);
    void Join(int relID1,int col1,int relID2,int colID2,Query *Q);
    void sortedJoin(int relID1,int col1,int relID2,int colID2,Query *Q);
    void Join(int relID1,int col1,JoinArray *array2,int relID2,int colID2,Query *Q);
    void sortedJoin(int relID1,int col1,JoinArray *array2,int relID2,int colID2,Query *Q);
    uint64_t get_sum(int relID,int colID);
    void print();
};


#endif
