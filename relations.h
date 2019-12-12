#ifndef PROJECT_RELATIONS_H
#define PROJECT_RELATIONS_H

#include "relation.h"
#include "list.h"

char* concat(const char *s1, const char *s2);


class Relations {
    int *query_rels;
    Relation **rels;
    int sz;
public:
    Relations(char *filename);
    //for debug
    Relations(Relation **rels,int sz) {
        this->rels = rels;
        this->sz = sz;
    }
    Relation *relation(int i);
    int getSize();
    ~Relations();
    array *get_col(int relation,uint64_t column);
    list *filter(int array,uint64_t column1,uint64_t column2);
    list *equal(int array,uint64_t column,uint64_t value);
    list *grater_than(int array,uint64_t column,uint64_t value);
    list *less_than(int array,uint64_t column,uint64_t value);
    bool filter(int array,uint64_t row,uint64_t column1,uint64_t column2);
    bool filter(int array1,int array2,uint64_t row1,uint64_t row2,uint64_t column1,uint64_t column2);
    bool equal(int array,uint64_t column,uint64_t value,uint64_t row);
    bool grater_than(int array,uint64_t column,uint64_t value,uint64_t row);
    bool less_than(int array,uint64_t column,uint64_t value,uint64_t row);
    uint64_t get_value(int array,uint64_t row,int column);
    void set_query_rels(int *from_arrays){
        query_rels=from_arrays;
    }
    uint64_t get_relRows(uint64_t rel){
        return rels[query_rels[rel]]->getRows();
    }
    uint64_t *get_column(uint64_t rel,uint64_t col){
        return rels[query_rels[rel]]->get_col(col);
    }
};


#endif