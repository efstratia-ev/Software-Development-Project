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
    array *sortRel(int col);
public:
    //add rels parameter in constructor
    void setrel(int ar);
    JoinArray(Relations *r);
    void set_currentColumn(int column); //antikatastithike apo tin setrel(se diaforetika simia)
    //giati o pinakas 1 edo mporei na einai sti thesi 0
    uint64_t get_value(int i);
    void set_value(int i,uint64_t val);
    void insert_row(uint64_t i,uint64_t *row);
    void update_array(list *results,int id);
    void update_array(list *results,JoinArray *array2);
    void create_array(list *results,int id);
    void create_array(list *results,int id1,int id2);
    int get_column(int arrayID);
    void filter_update(list *results);
    void compare(int arrayID,uint64_t column1,uint64_t column2);
    list *getcompare(int arrayID,uint64_t column1,uint64_t column2);
    void compare(int arrayID1,uint64_t column1,int arrayID2,uint64_t column2);
    void grater_than(uint64_t column,uint64_t value);
    void less_than(uint64_t column,uint64_t value);
    void equal(uint64_t column,uint64_t value);
    bool exists(int arrayID);
    list *Join(int relID1,int col1,int relID2,int colID2);
    list *Join(int relID1,int col1,JoinArray *array2,int relID2,int colID2);
    void print(){
        for(int i=0; i<numRels; i++) cout<<relationIDs[i]<<"\t";
        cout<<endl;
        for(int j=0; j<size; j++){
            for(int i=0; i<numRels; i++)
                cout<<Array[j][i]<<"\t";
            cout<<endl;
        }
    }
    uint64_t get_sum(int relID,int colID){
        uint64_t sum=0;
        setrel(relID);
        for(int i=0; i<size; i++){
            sum+=rels->get_value(relToBeJoined,get_value(i),colID);
        }
        return sum;
    }
};


#endif
