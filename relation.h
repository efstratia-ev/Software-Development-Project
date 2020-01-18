#ifndef PROJECT_RELATION_H
#define PROJECT_RELATION_H

#include <iostream>
#include <fstream>
#include "rows_array.h"
#include "stats.h"

//using namespace std;

void handle_errror(char *msg);

class Relation {
    uint64_t *data;  //relations data by column
    uint64_t rows;
    uint64_t cols;
    uint64_t fileSz{};
    stats **statistics;
    public:
    explicit Relation(const char *filename); //loads file into memory with mmap
    Relation(uint64_t *data,uint64_t rows,uint64_t cols);
    Relation() {};
    ~Relation();
    uint64_t value(uint64_t row,uint64_t col); //return value at row 'row' and col 'col' (zero based)
    uint64_t getRows();
    uint64_t getCols();
    uint64_t *getData() { return data; }
    uint64_t *get_col(uint64_t i);
    uint64_t *col(uint64_t i,uint64_t &size);
    int compare_values(uint64_t row,uint64_t column1,uint64_t column2);
    int compare(uint64_t row,uint64_t column,uint64_t value);
    stats *calculateStats(int col);
    stats *getColStats(int i) { return statistics[i]; }
    void setStats(int col,stats *sts) { statistics[col] = sts; }
    stats *getStatsCopy(int col) { 
        auto st = statistics[col];
        return new stats(st->getMin(),st->getMax(),st->getBitMap(),st->getTotalValues(),st->getDistinctValues());
    }
    stats **getStatsCopy() {
        auto sts = new stats*[cols];
        for (int i = 0; i < cols; i++) {
            sts[i] = new stats(statistics[i]->getMin(),statistics[i]->getMax(),statistics[i]->getBitMap(),statistics[i]->getTotalValues(),statistics[i]->getDistinctValues());
        }
        return sts;
    }
    void setStats(stats **sts) { statistics = sts; }
    void printStats() {
        for (int i = 0; i < cols; i++) {
            statistics[i]->print();
        }

    }
    void delete_map();

};


#endif