#ifndef PROJECT_RELATION_H
#define PROJECT_RELATION_H

#include "array.h"

using namespace std;

void handle_errror(char *msg);

class Relation {
    uint64_t *data;
    uint64_t rows;
    uint64_t cols;
    uint64_t fileSz{};
    public:
    explicit Relation(const char *filename); //loads file into memory with mmap
    Relation(uint64_t *data,uint64_t rows,uint64_t cols);
    ~Relation();
    uint64_t value(uint64_t row,uint64_t col); //return value at row 'row' and col 'col' (zero based)
    uint64_t getRows();
    uint64_t getCols();
    uint64_t *get_col(uint64_t i);
    uint64_t *col(uint64_t i,uint64_t &size);
    int compare_values(uint64_t row,uint64_t column1,uint64_t column2);
    int compare(uint64_t row,uint64_t column,uint64_t value);
};


#endif