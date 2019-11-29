#ifndef PROJECT2019_RESULTSARRAY_H
#define PROJECT2019_RESULTSARRAY_H


#include "list.h"

class ResultsArray {
    int size;
    int rowIDs;
    int **Array;
    int *arrayIDs;
    int currentColumn;
public:
    ResultsArray(int s,int r);
    void set_currentColumn(int column);
    int get_value(int i);
    void insert_row(int i,int *row);
    void update_array(list *results,int id);
    void update_array(list *results,ResultsArray *array2);
    void create_array(list *results,int id);
};


#endif
