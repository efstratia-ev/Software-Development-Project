#include "ResultsArray.h"

ResultsArray::ResultsArray(uint64_t s, uint64_t r) {
    size=s;
    rowIDs=r;
    Array=new uint64_t*[size];
}

void ResultsArray::set_currentColumn(int column) {
    currentColumn=column;
}

uint64_t ResultsArray::get_value(uint64_t i) {
    return Array[i][currentColumn];
}

void ResultsArray::insert_row(uint64_t i, uint64_t *row) {
    Array[i]=row;
}

void ResultsArray::update_array(list *results,int id) {
    uint64_t new_size=results->get_size();
    uint64_t **new_array=new uint64_t*[new_size];
    rowids *rows;
    int *new_arrayID=new int[rowIDs+1];
    int n=-1;
    for(int j=0; j<rowIDs; j++){
        if(n>0){
            new_arrayID[j+1]=arrayIDs[j];
            arrayIDs[j]=j+1;
        }
        else if(arrayIDs[j]>id){
            new_arrayID[j]=id;
            n=j;
        }
        else{
            new_arrayID[j]=arrayIDs[j];
            arrayIDs[j]=j;
        }

    }
    for(uint64_t i=0; i<new_size; i++){
        new_array[i]=new uint64_t[rowIDs+1];
        rows=results->pop();
        for(uint64_t j=0; j<rowIDs; j++){
            new_array[i][j]=Array[rows->rowid1][j];
        }
        new_array[arrayIDs[i]][n]=rows->rowid2;
        delete rows;
    }
    for(uint64_t i=0; i<size; i++)delete[] Array[i];
    delete[] Array;
    size=new_size;
    rowIDs+=1;
    delete[] arrayIDs;
    arrayIDs=new_arrayID;
}

void ResultsArray::update_array(list *results, ResultsArray *array2) {
    uint64_t new_size=results->get_size();
    uint64_t **new_array=new uint64_t*[new_size];
    rowids *rows;
    int *new_arrayID=new int[rowIDs+1];
    int n=-1;
    for(int j=0; j<rowIDs; j++){
        if(n>0){
            new_arrayID[j+1]=arrayIDs[j];
            arrayIDs[j]=j+1;
        }
        else if(arrayIDs[j]>array2->arrayIDs[0]){
            new_arrayID[j]=array2->arrayIDs[0];
            n=j;
        }
        else{
            new_arrayID[j]=arrayIDs[j];
            arrayIDs[j]=j;
        }

    }
    for(uint64_t i=0; i<new_size; i++){
        new_array[i]=new uint64_t[rowIDs+1];
        rows=results->pop();
        for(uint64_t j=0; j<rowIDs; j++){
            new_array[i][j]=Array[rows->rowid1][j];
        }
        new_array[arrayIDs[i]][n]=array2->Array[rows->rowid2][0];
        delete rows;
    }
    for(uint64_t i=0; i<size; i++)delete[] Array[i];
    delete[] Array;
    size=new_size;
    rowIDs+=1;
    delete[] arrayIDs;
    arrayIDs=new_arrayID;
}

void ResultsArray::create_array(list *results,int id) {
    uint64_t size = results->get_size();
    Array = new uint64_t *[size];
    arrayIDs = new int[1];
    arrayIDs[0] = id;
    for (uint64_t i = 0; i < size; i++) {
        Array[i] = new uint64_t[1];
        Array[i][0] = results->pop_element();
    }
}