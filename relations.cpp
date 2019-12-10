#include <fstream>
#include <cstring>
#include "relations.h"


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
    if (suffix != nullptr) {
        prefix = (char *)malloc((suffix-filename+1) * sizeof(char));
        strncpy(prefix,filename,suffix-filename+1);
    }
    const char *fname;
    count = 0;
    while (getline(_infile,line)) {
        if (suffix != nullptr)
            fname = concat(prefix,line.c_str());
        else
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

array *Relations::get_column(int relation, uint64_t column) {
    return rels[query_rels[relation]]->col(column);
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