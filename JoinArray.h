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
    int *newrelationIDs{};
    uint64_t new_size;
    uint64_t **new_array;
    int n;
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
    void keep_new_results();
    void add_relation(int id,uint64_t newsize);
    void add_relations(int id1,int id2,uint64_t newsize);
    void update_array(list *results,uint64_t offset);
    void update_array(list *results,JoinArray *array2,uint64_t offset);
    void create_array(list *results,int id);
    void create_array(list *results,uint64_t offset);
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
    void set_array(uint64_t i,uint64_t num1,uint64_t num2){
        Array[i][0]=num1;
        Array[i][1]=num2;
    }
    void set_newArray(uint64_t i,uint64_t num1,uint64_t num2){
        for(uint64_t j=0; j<numRels; j++){
            int temp=relationIDs[j];
            new_array[temp][i]=Array[j][num1]; //keep from the old array the rows that are extracted from the list
        }
        new_array[n][i]=num2;   //in every row add the value of the new column (new relation added)
    }
    void set_newArray(uint64_t i,uint64_t num1,uint64_t num2,JoinArray *array2){
        for(uint64_t j=0; j<numRels; j++){
            int temp=relationIDs[j];
            new_array[temp][i]=Array[j][num1]; //keep from the old array the rows that are extracted from the list
        }
        new_array[n][i]=array2->Array[0][num2];   //in every row add the value of the new column (new relation added)
    }
};


#endif
