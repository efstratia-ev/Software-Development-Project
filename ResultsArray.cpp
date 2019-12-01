#include "ResultsArray.h"

ResultsArray::ResultsArray(uint64_t s, uint64_t r) {
    size=s;
    columnIDs=r;
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
    int *new_arrayID=new int[columnIDs+1];
    int n=-1;
    for(int j=0; j<columnIDs; j++){
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
        new_array[i]=new uint64_t[columnIDs+1];
        rows=results->pop();
        for(uint64_t j=0; j<columnIDs; j++){
            if(j!=n) new_array[i][j]=Array[rows->rowid1][j];
        }
        new_array[arrayIDs[i]][n]=rows->rowid2;
        delete rows;
    }
    for(uint64_t i=0; i<size; i++)delete[] Array[i];
    delete[] Array;
    size=new_size;
    columnIDs+=1;
    delete[] arrayIDs;
    arrayIDs=new_arrayID;
}

void ResultsArray::update_array(list *results, ResultsArray *array2) {
    uint64_t new_size=results->get_size();
    uint64_t **new_array=new uint64_t*[new_size];
    rowids *rows;
    int *new_arrayID=new int[columnIDs+1];
    int n=-1;
    for(int j=0; j<columnIDs; j++){
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
        new_array[i]=new uint64_t[columnIDs+1];
        rows=results->pop();
        for(uint64_t j=0; j<columnIDs; j++){
            if(j!=n) new_array[i][j]=Array[rows->rowid1][j];
        }
        new_array[arrayIDs[i]][n]=array2->Array[rows->rowid2][0];
        delete rows;
    }
    for(uint64_t i=0; i<size; i++)delete[] Array[i];
    delete[] Array;
    size=new_size;
    columnIDs+=1;
    delete[] arrayIDs;
    arrayIDs=new_arrayID;
}

void ResultsArray::create_array(list *results,int id) {
    uint64_t sz = results->get_size();
    Array = new uint64_t *[sz];
    arrayIDs = new int[1];
    arrayIDs[0] = id;
    for (uint64_t i = 0; i < sz; i++) {
        Array[i] = new uint64_t[1];
        Array[i][0] = results->pop_element();
    }
}

int ResultsArray::get_column(int arrayID) {
    for(int i=0; i<columnIDs; i++)
        if(arrayID==arrayIDs[i]) return i;
}

void ResultsArray::filter_update(list *results) {
    uint64_t new_size=results->get_size();
    uint64_t **new_array=new uint64_t*[new_size];
    rowids *rows;

    for(uint64_t i=0; i<new_size; i++){
        new_array[i]=new uint64_t[columnIDs];
        rows=results->pop();
        for(uint64_t j=0; j<columnIDs; j++){
            new_array[i][j]=Array[rows->rowid1][j];
        }
    }
    for(uint64_t i=0; i<size; i++)delete[] Array[i];
    delete[] Array;
    size=new_size;
    delete[] arrayIDs;
}

void ResultsArray::compare(int arrayID, uint64_t column1, uint64_t column2, Relations *Data) {
    int column=get_column(arrayID);
    list *results=new list();
    for(uint64_t i=0; i<size; i++){
        if(Data->filter(arrayID,Array[column][i],column1,column2))
            results->add(Array[column][i]);
    }
    filter_update(results);
}

void ResultsArray::compare(int arrayID1, uint64_t column1, int arrayID2, uint64_t column2, Relations *Data) {
    int c1=get_column(arrayID1),c2=get_column(arrayID2);
    list *results=new list();
    for(uint64_t i=0; i<size; i++){
        if(Data->filter(arrayID1,arrayID2,Array[column1][i],Array[column2][i],column1,column2))
            results->add(i);
    }
    filter_update(results);
}

bool ResultsArray::exists(int arrayID) {
    for(int i=0; i<columnIDs; i++){
        if(arrayIDs[i]==arrayID) return true;
    }
    return false;
}
