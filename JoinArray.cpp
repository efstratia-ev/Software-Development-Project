#include "JoinArray.h"
#include "array.h"
#include "sort.h"

JoinArray::JoinArray(Relations *r) {
    rels=r;
    numRels=0;
}

uint64_t JoinArray::get_value(uint64_t i) {
    return Array[relToBeJoined][i];
}

void JoinArray::update_array(list *results,int id) {
    uint64_t new_size=results->get_size();
    uint64_t **new_array=new uint64_t*[numRels+1];
    for(int j=0; j<numRels+1; j++) new_array[j]=new uint64_t[new_size];
    rowids *rows;
    int *new_arrayID=new int[numRels+1];
    int n=-1;
    for(int j=0; j<numRels; j++){
        if(n<0 && relationIDs[j]>id){
            new_arrayID[j]=id;
            n=j;
        }
        if(n>=0){
            new_arrayID[j+1]=relationIDs[j];
            relationIDs[j]=j+1;
        }
        else{
            new_arrayID[j]=relationIDs[j];
            relationIDs[j]=j;
        }
    }
    if(n==-1){
        n=numRels;
        new_arrayID[n]=id;
    }
    for(uint64_t i=0; i<new_size; i++){
        rows=results->pop();
        for(uint64_t j=0; j<numRels; j++){
            int temp=relationIDs[j];
            new_array[temp][i]=Array[j][rows->rowid1];
        }
        new_array[n][i]=rows->rowid2;
    }
    for(uint64_t i=0; i<numRels; i++)delete[] Array[i];
    delete[] Array;
    Array=new_array;
    size=new_size;
    numRels+=1;
    delete[] relationIDs;
    relationIDs=new_arrayID;
}

void JoinArray::update_array(list *results, JoinArray *array2) {
    uint64_t new_size=results->get_size();
    uint64_t **new_array=new uint64_t*[numRels+1];
    for(int j=0; j<numRels+1; j++) new_array[j]=new uint64_t[new_size];
    rowids *rows;
    int *new_arrayID=new int[numRels+1];
    int n=-1;
    for(int j=0; j<numRels; j++){
        if(n<0 && relationIDs[j]>array2->relationIDs[0]){
            new_arrayID[j]=array2->relationIDs[0];
            n=j;
        }
        if(n>=0){
            new_arrayID[j+1]=relationIDs[j];
            relationIDs[j]=j+1;
        }
        else{
            new_arrayID[j]=relationIDs[j];
            relationIDs[j]=j;
        }
    }
    if(n==-1){
        n=numRels;
        new_arrayID[n]=array2->relationIDs[0];
    }
    for(uint64_t i=0; i<new_size; i++){
        rows=results->pop();
        for(uint64_t j=0; j<numRels; j++){
            new_array[relationIDs[j]][i]=Array[j][rows->rowid1];
        }
        new_array[n][i]=array2->Array[0][rows->rowid2];
    }
    for(uint64_t i=0; i<numRels; i++) delete[] Array[i];
    delete[] Array;
    Array=new_array;
    size=new_size;
    numRels+=1;
    delete[] relationIDs;
    relationIDs=new_arrayID;
}

void JoinArray::create_array(list *results,int id) {
    size = results->get_size();
    Array = new uint64_t *[1];
    Array[0] = new uint64_t[size];
    relationIDs = new int[1];
    relationIDs[0] = id;
    numRels=1;
    for (uint64_t i = 0; i < size; i++)
        Array[0][i] = results->pop_element();
}
void JoinArray::create_array(list *results,int id1,int id2) {
    size = results->get_size();
    Array = new uint64_t *[2];
    Array[0] = new uint64_t[size];
    Array[1] = new uint64_t[size];
    relationIDs = new int[2];
    bool op=false;
    numRels=2;
    if(id1<id2) {
        relationIDs[0] = id1;
        relationIDs[1] = id2;
    }
    else{
        relationIDs[0] = id2;
        relationIDs[1] = id1;
        op=true;
    }
    for (uint64_t i = 0; i < size; i++) {
        rowids* temp= results->pop();
        if(op){
            Array[0][i] = temp->rowid2;
            Array[1][i] = temp->rowid1;
        }
        else{
            Array[0][i] = temp->rowid1;
            Array[1][i] = temp->rowid2;
        }
    }
}

int JoinArray::get_column(int arrayID) {
    for(int i=0; i<numRels; i++)
        if(arrayID==relationIDs[i]) return i;
}

void JoinArray::filter_update(list *results) {
    results->restart_current();
    uint64_t new_size=results->get_size();
    uint64_t **new_array=new uint64_t*[numRels];
    for(uint64_t i=0; i<numRels; i++)
        new_array[i]=new uint64_t[new_size];
    rowids *rows;

    uint64_t temp;
    for(uint64_t i=0; i<new_size; i++){
        temp=results->pop_element();
        for(uint64_t j=0; j<numRels; j++){
            new_array[j][i]=Array[j][temp];
        }
    }
    for(uint64_t i=0; i<numRels; i++)delete[] Array[i];
    delete[] Array;
    Array=new_array;
    size=new_size;
}

void JoinArray::compare(int arrayID, uint64_t column1, uint64_t column2) {
    int column=get_column(arrayID);
    list *results=new list();
    for(uint64_t i=0; i<size; i++){
        if(rels->filter(arrayID,Array[column][i],column1,column2))
            results->add(i);
    }
    results->restart_current();
    filter_update(results);
}

list *JoinArray::getcompare(int arrayID, uint64_t column1, uint64_t column2) {
    int column=get_column(arrayID);
    list *results=new list();
    for(uint64_t i=0; i<size; i++){
        if(rels->filter(arrayID,Array[column][i],column1,column2))
            results->add(Array[column][i]);
    }
    results->restart_current();
    return results;
}

void JoinArray::compare(int arrayID1, uint64_t column1, int arrayID2, uint64_t column2) {
    int c1=get_column(arrayID1),c2=get_column(arrayID2);
    list *results=new list();
    for(uint64_t i=0; i<size; i++){
        if(rels->filter(arrayID1,arrayID2,Array[c1][i],Array[c2][i],column1,column2))
            results->add(i);
    }
    results->restart_current();
    filter_update(results);
}

bool JoinArray::exists(int relation) {
    for(int i=0; i<numRels; i++){
        if(relationIDs[i]==relation)
            return true;
    }
    return false;
}

//wraps join and update. If array2 isn't nil it performs a join 
//with two JoinArrays. Otherwise performs join with one JoinArray
//and one relation (simple array).
void JoinArray::joinUpdate(int relID1,int col1,int relID2,int colID2,JoinArray *array2) {
    list *lst;
    if (array2 != nullptr) {
        lst = Join(relID1,col1,array2,relID2,colID2);
        update_array(lst,array2);
    } else {
        lst = Join(relID1,col1,relID2,colID2);
        update_array(lst,relID2);
    }
}

//relID1 is the relation that already exists in ResultsArray and we 
//will call sortRel on. relID2 is the relation that we are willing
//to add in ResultsArray after join.
list *JoinArray::Join(int relID1,int col1,int relID2,int colID2) {
    setrel(relID1);
    auto arr1 = sortRel(col1);
    auto arr2 = rels->get_column(relID2,colID2);
    sort(new radix(arr2->Size,arr2->Array));
    list *results=join(arr1,arr2);
    results->restart_current();
    return results;
}

list *JoinArray::Join(int relID1,int col1,JoinArray *array2,int relID2,int colID2) {
    setrel(relID1);
    auto arr1 = sortRel(col1);
    array2->setrel(relID2);
    auto arr2 = array2->sortRel(colID2);
    list *results=join(arr1,arr2);
    results->restart_current();
    return results;
}


//returns sorted rowIDs of relation 'rel' based on 
//column 'col' of 'rel'. Usually we call this
//before a join. This method is called only by Join
array *JoinArray::sortRel(int col) {
    uint64_t row;
    array *arr = new array(size);
    for (int i =0; i < size; i++) {
        row = get_value(i);
        arr->add( rels->get_value(relationIDs[relToBeJoined],row,col),i);
    }
    sort(new radix(arr->Size,arr->Array));
    return arr;
}

void JoinArray::grater_than(uint64_t column, uint64_t value) {
    list *results=new list();
    for(uint64_t i=0; i<size; i++){
        if(rels->grater_than(relationIDs[0],column,value,i))
            results->add(i);
    }
    filter_update(results);
}

void JoinArray::less_than(uint64_t column, uint64_t value) {
    list *results=new list();
    for(uint64_t i=0; i<size; i++){
        if(rels->less_than(relationIDs[0],column,value,i))
            results->add(i);
    }
    filter_update(results);
}

void JoinArray::equal(uint64_t column, uint64_t value) {
    list *results=new list();
    for(uint64_t i=0; i<size; i++){
        if(rels->equal(relationIDs[0],column,value,i))
            results->add(i);
    }
    filter_update(results);
}

void JoinArray::setrel(int ar) {
    for(int i=0; i<numRels; i++){
        if(relationIDs[i]==ar) relToBeJoined=i;
    }
}

uint64_t JoinArray::get_sum(int relID, int colID) {
    uint64_t sum=0;
    setrel(relID);
    for(uint64_t i=0; i<size; i++){
        sum+=rels->get_value(relID,get_value(i),colID);
    }
    return sum;
}

int JoinArray::getNumRels() {
    return numRels;
}

int JoinArray::getSize() {
    return size;
}

uint64_t **JoinArray::getArray() {
    return Array;
}



