#ifndef PROJECT2019_RESULTSARRAY_H
#define PROJECT2019_RESULTSARRAY_H


#include "list.h"

class ResultsArray {
    uint64_t size;
    int rowIDs;
    uint64_t **Array;
    int *arrayIDs;
    int currentColumn;
public:
    ResultsArray(uint64_t s,uint64_t r);
    void set_currentColumn(int column);
    uint64_t get_value(uint64_t i);
    void insert_row(uint64_t i,uint64_t *row);
    void update_array(list *results,int id);
    void update_array(list *results,ResultsArray *array2);
    void create_array(list *results,int id);
};


#endif
