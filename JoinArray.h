#ifndef PROJECT2019_RESULTSARRAY_H
#define PROJECT2019_RESULTSARRAY_H


#include "list.h"
#include "relations.h"

class JoinArray {
    Relations *rels; //Join needs this
    uint64_t size; //size of joinResults
    uint64_t **Array;
    int numRels; //how many rels are in JoinArray
    int *relationIDs; //which rels 
    //relation belonging to relationIDs array that we 
    //will apply the next join on.
    int relToBeJoined; 
    array *sortRel(int relToBeJoined,int col);
public:
    //add rels parameter in constructor
    JoinArray(uint64_t s,uint64_t r);
    void set_currentColumn(int column);
    uint64_t get_value(int i);
    void set_value(int i,uint64_t val);
    void insert_row(uint64_t i,uint64_t *row);
    void update_array(list *results,int id);
    void update_array(list *results,JoinArray *array2);
    void create_array(list *results,int id);
    int get_column(int arrayID);
    void filter_update(list *results);
    void compare(int arrayID,uint64_t column1,uint64_t column2,Relations *Data);
    void compare(int arrayID1,uint64_t column1,int arrayID2,uint64_t column2,Relations *Data);
    bool exists(int arrayID);
    list *Join(int relID1,int col1,int relID2,int colID2);
};


#endif
