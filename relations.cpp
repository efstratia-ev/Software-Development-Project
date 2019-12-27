#include <fstream>
#include <cstring>
#include "relations.h"

using namespace std;

char *concat(const char *s1, const char *s2) {
    char *result = (char *)malloc(strlen(s1) + strlen(s2) + 1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

Relation *Relations::relation(int i) {
    return rels[query_rels[i]];
}

Relations::Relations(char *filename) {
    ifstream infile(filename);
    string line;
    int count = 0;
    while (getline(infile,line)) {
        count++;
    }
    sz = count;
    rels = new Relation*[count];
    ifstream _infile(filename);
    char *suffix = strrchr(filename,'/');
    char *prefix;
    count = 0;
    if (suffix) {
        prefix = new char[suffix-filename+2];
        strncpy(prefix,filename,suffix-filename+1);
        prefix[suffix-filename+1]='\0';
        char *fname;
        while (getline(_infile,line)) {
            fname = concat(prefix,line.c_str());
            rels[count] = new Relation(fname);
            count++;
            free(fname);
        }

        delete[] prefix;
        return;
    }
    const char *fname;
    while (getline(_infile,line)) {
        fname = line.c_str();
        rels[count] = new Relation(fname);
        count++;
    }
}

int Relations::getSize() { return sz; }

Relations::~Relations() {
    for(int i=0; i<sz; i++) delete rels[i];
    delete[] rels;
}

list *Relations::filter(int array, uint64_t column1, uint64_t column2) {
    list *results=new list();
    int size=rels[query_rels[array]]->getRows();
    for(int i=0; i<size; i++){
        if(rels[query_rels[array]]->compare_values(i,column1,column2)==0)
            results->add(i);
    }
    results->restart_current();
    return results;
}

list *Relations::equal(int array, uint64_t column, uint64_t value) {
    list *results=new list();
    int size=rels[query_rels[array]]->getRows();
    for(int i=0; i<size; i++){
        if(rels[query_rels[array]]->compare(i,column,value)==0)
            results->add(i);
    }
    results->restart_current();
    return results;
}

list *Relations::grater_than(int array, uint64_t column, uint64_t value) {
    list *results=new list();
    int size=rels[query_rels[array]]->getRows();
    for(int i=0; i<size; i++){
        if(rels[query_rels[array]]->compare(i,column,value)>0)
            results->add(i);
    }
    results->restart_current();
    return results;
}

list *Relations::less_than(int array, uint64_t column, uint64_t value) {
    list *results=new list();
    int size=rels[query_rels[array]]->getRows();
    for(int i=0; i<size; i++){
        if(rels[query_rels[array]]->compare(i,column,value)<0)
            results->add(i);
    }
    results->restart_current();
    return results;
}

bool Relations::filter(int array, uint64_t row, uint64_t column1, uint64_t column2) {
    return rels[query_rels[array]]->compare_values(row,column1,column2)==0;
}

bool Relations::filter(int array1, int array2, uint64_t row1, uint64_t row2, uint64_t column1, uint64_t column2) {
    return rels[query_rels[array1]]->value(row1,column1)==rels[query_rels[array2]]->value(row2,column2);
}

bool Relations::equal(int array, uint64_t column, uint64_t value,uint64_t row) {
    return (rels[query_rels[array]]->compare(row,column,value)==0);
}

bool Relations::grater_than(int array, uint64_t column, uint64_t value,uint64_t row) {
    return (rels[query_rels[array]]->compare(row,column,value)>0);
}

bool Relations::less_than(int array, uint64_t column, uint64_t value,uint64_t row) {
    return (rels[query_rels[array]]->compare(row,column,value)>0);
}

uint64_t Relations::get_value(int array, uint64_t row, int column) {
    return rels[query_rels[array]]->value(row,column);
}

//point to the same data and copy statistics
Relations::Relations(Relation **rels, int sz) {
    this->sz = sz;
    this->rels = new Relation*[sz];
    for (int i = 0; i <sz; i++) {
        this->rels[i] = new Relation(rels[i]->getData(),rels[i]->getRows(),rels[i]->getCols());
        this->rels[i]->setStats(rels[i]->getStatsCopy()); 
    }
}

void Relations::set_query_rels(int *from_arrays) {
    query_rels=from_arrays;
}

uint64_t Relations::get_relRows(uint64_t rel) {
    return rels[query_rels[rel]]->getRows();
}

uint64_t *Relations::get_column(uint64_t rel, uint64_t col) {
    return rels[query_rels[rel]]->get_col(col);
}
