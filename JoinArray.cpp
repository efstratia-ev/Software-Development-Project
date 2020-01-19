#include <iostream>
#include "JoinArray.h"
#include "rows_array.h"
#include "sort.h"

using namespace std;

JoinArray::JoinArray(Relations *r) {
    rels=r;
    numRels=0;
    newrelationIDs= nullptr;
}

uint64_t JoinArray::get_value(uint64_t i) {
    return Array[relToBeJoined][i];
}

void JoinArray::update_array(list *results,uint64_t offset) {
    uint64_t i=offset;
    rowids *rows;
    while((rows=results->pop())){
        for(uint64_t j=0; j<numRels; j++){
            int temp=relationIDs[j];
            new_array[temp][i]=Array[j][rows->rowid1]; //keep from the old array the rows that are extracted from the list
        }
        new_array[n][i]=rows->rowid2;   //in every row add the value of the new column (new relation added)
        delete rows;
        i++;
    }
    delete results;
}

//it is similar to the above function, the only difference is that the new results are from array2
void JoinArray::update_array(list *results, JoinArray *array2,uint64_t offset) {
    uint64_t i=offset;
    rowids *rows;
    while((rows=results->pop())){
        for(uint64_t j=0; j<numRels; j++){
            new_array[relationIDs[j]][i]=Array[j][rows->rowid1];
        }
        new_array[n][i]=array2->Array[0][rows->rowid2];
        delete rows;
        i++;
    }
    delete results;
}

//creates the array after the use of a filter
void JoinArray::create_array(list *results,int id) {
    size = results->get_size();
    Array = new uint64_t *[1];
    Array[0] = new uint64_t[size];
    relationIDs = new int[1];
    relationIDs[0] = id;
    numRels=1;
    for (uint64_t i = 0; i < size; i++)
        Array[0][i] = results->pop_element(); //extract every element from the list to the array
    delete results;
}

//creates the array after the first join (if no filter is used before)
void JoinArray::create_array(list *results,uint64_t offset) {
    rowids* temp= results->pop(); //extract every element from the list to the array
    uint64_t i=offset;
    while(temp){
        Array[0][i] = temp->rowid1;
        Array[1][i] = temp->rowid2;
        temp=results->pop();
        i++;
    }
    delete results;
}

int JoinArray::get_column(int arrayID) { //return in which column is relation with arrayID
    for(int i=0; i<numRels; i++)
        if(arrayID==relationIDs[i]) return i;
    return -1;
}


//after a join in two tables that were already in join array keeps only the rows that are on the list
void JoinArray::filter_update(list *results) {
    results->restart_current();
    uint64_t new_size=results->get_size();
    auto new_array=new uint64_t*[numRels];
    for(uint64_t i=0; i<numRels; i++)
        new_array[i]=new uint64_t[new_size];

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
    delete results;
}

void JoinArray::compare(int arrayID, uint64_t column1, uint64_t column2) { //compares arrays column1 with column2
    int column=get_column(arrayID);
    list *results=new list();
    for(uint64_t i=0; i<size; i++){
        if(rels->filter(arrayID,Array[column][i],column1,column2))
            results->add(i); //add to the list elements with the same value
    }
    results->restart_current();
    filter_update(results);
}


void JoinArray::compare(int arrayID1, uint64_t column1, int arrayID2, uint64_t column2) {
    int c1=get_column(arrayID1),c2=get_column(arrayID2);
    list *results=new list();
    for(uint64_t i=0; i<size; i++){
        if(rels->filter(arrayID1,arrayID2,Array[c1][i],Array[c2][i],column1,column2))
            results->add(i);  //add to the list elements with the same value
    }
    results->restart_current();
    filter_update(results);
}

bool JoinArray::exists(int relation) { //check if relation exists in join array
    for(int i=0; i<numRels; i++){
        if(relationIDs[i]==relation)
            return true;
    }
    return false;
}

//wraps join and update. If array2 isn't nil it performs a join 
//with two JoinArrays. Otherwise performs join with one JoinArray
//and one relation (simple array).

//relID1 is the relation that already exists in ResultsArray and we 
//will call sortRel on. relID2 is the relation that we are willing
//to add in ResultsArray after join.
void JoinArray::Join(int relID1,int colID1,int relID2,int colID2,Query *Q) {
    setrel(relID1);
    sem_t *sem=new sem_t;
    sem_init(sem, 1, 0);
    auto arr1 = sort(sem,Q,new sorted_radix(size,Array[relToBeJoined],rels->get_column(relID1,colID1)));
    auto arr2 = sort(sem,Q,new radix(rels->get_relRows(relID2),rels->get_column(relID2,colID2)));
    js->Schedule(new MergeJob(Q,arr1,arr2,false,rels->get_column(relID1,colID1),rels->get_column(relID2,colID2),relID1,relID2),sem,2*NUMJOBS);
}

void JoinArray::sortedJoin(int relID1,int colID1,int relID2,int colID2,Query *Q) {
    setrel(relID1);
    sem_t *sem=new sem_t;
    sem_init(sem, 1, 0);
    auto arr1 = new rows_array(size,Array[relToBeJoined]);
    auto arr2 = sort(sem,Q,new radix(rels->get_relRows(relID2),rels->get_column(relID2,colID2)));
    js->Schedule(new MergeJob(Q,arr1,arr2,true,rels->get_column(relID1,colID1),rels->get_column(relID2,colID2),relID1,relID2),sem,NUMJOBS);

}

void JoinArray::Join(int relID1,int colID1,JoinArray *array2,int relID2,int colID2,Query *Q) {
    setrel(relID1);
    sem_t *sem=new sem_t;
    sem_init(sem, 1, 0);
    auto arr1 = sort(sem,Q,new sorted_radix(size,Array[relToBeJoined],rels->get_column(relID1,colID1)));
    array2->setrel(relID2);
    auto arr2 = sort(sem,Q,new sorted_radix(array2->size,array2->Array[array2->relToBeJoined],rels->get_column(relID2,colID2)));
    js->Schedule(new MergeJob(Q,arr1,arr2,false,rels->get_column(relID1,colID1),rels->get_column(relID2,colID2),relID1,relID2),sem,2*NUMJOBS);

}

void JoinArray::sortedJoin(int relID1,int colID1,JoinArray *array2,int relID2,int colID2,Query *Q) {
    setrel(relID1);
    sem_t *sem=new sem_t;
    sem_init(sem, 1, 0);
    auto arr1 = new rows_array(size,Array[relToBeJoined]);
    array2->setrel(relID2);
    auto arr2 = sort(sem,Q,new sorted_radix(array2->size,array2->Array[array2->relToBeJoined],rels->get_column(relID2,colID2)));
    js->Schedule(new MergeJob(Q,arr1,arr2,true,rels->get_column(relID1,colID1),rels->get_column(relID2,colID2),relID1,relID2),sem,NUMJOBS);
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
        if(relationIDs[i]==ar){
            relToBeJoined=i;
            break;
        }
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

void JoinArray::print() {
   cout << "results:" << endl;
   auto arr = Array;
   for (int i =0; i < size; i++) {
     cout << endl;
     for (int j =0; j < numRels; j++)
        cout << arr[i][j] << ",";
   }
   cout << endl;
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

JoinArray::~JoinArray() {
    for(int i=0; i<numRels; i++) delete[] Array[i];
    delete[] Array;
    delete[] relationIDs;
}

void JoinArray::add_relation(int id, uint64_t newsize) {
    new_size=newsize;
    new_array=new uint64_t*[numRels+1];
    for(int j=0; j<numRels+1; j++) new_array[j]=new uint64_t[new_size];
    newrelationIDs=new int[numRels+1]; //will contain every element of relationIDs and id
    n=-1;
    for(int j=0; j<numRels; j++){   //add elements in the right order
        if(n<0 && relationIDs[j]>id){
            newrelationIDs[j]=id;
            n=j;
        }
        if(n>=0){
            newrelationIDs[j+1]=relationIDs[j];
            relationIDs[j]=j+1;
        }
        else{
            newrelationIDs[j]=relationIDs[j];
            relationIDs[j]=j;
        }
    }
    if(n==-1){  //new element is the largest and it is not added to the array
        n=numRels;
        newrelationIDs[n]=id;
    }
}

void JoinArray::add_relations(int id1, int id2, uint64_t newsize) {
    size=newsize;
    Array = new uint64_t *[2];
    Array[0] = new uint64_t[size];
    Array[1] = new uint64_t[size];
    relationIDs = new int[2];
    numRels=2;
    relationIDs[0] = id1;
    relationIDs[1] = id2;
    newrelationIDs= nullptr;
}

void JoinArray::keep_new_results() {
    if(!newrelationIDs) return;
    delete relationIDs;
    relationIDs=newrelationIDs;
    newrelationIDs= nullptr;
    for(int i=0; i<numRels; i++) delete[] Array[i];
    delete[] Array;
    Array=new_array;
    new_array=nullptr;
    size=new_size;
    numRels++;
}

//The following methods are the ones used at part2 when we had no parallelism

//relID1 is the relation that already exists in ResultsArray and we 
//will call sortRel on. relID2 is the relation that we are willing
//to add in ResultsArray after join.
list *JoinArray::Join(int relID1,int colID1,int relID2,int colID2) {
    setrel(relID1);
    auto arr1 = sort(new radix(size,Array[relToBeJoined],rels->get_column(relID1,colID1)));
    auto arr2 = sort(new radix(rels->get_relRows(relID2),rels->get_column(relID2,colID2)));
    list *results=join(arr1,arr2,rels->get_column(relID1,colID1),rels->get_column(relID2,colID2));
    results->restart_current();
    delete[] arr1->Array;
    delete[] arr2->Array;
    delete arr1;
    delete arr2;
    return results;
}

list *JoinArray::sortedJoin(int relID1,int colID1,int relID2,int colID2) {
    setrel(relID1);
    auto arr1 = new rows_array(size,Array[relToBeJoined]);
    auto arr2 = sort(new radix(rels->get_relRows(relID2),rels->get_column(relID2,colID2)));
    list *results=sortedjoin(arr1,arr2,rels->get_column(relID1,colID1),rels->get_column(relID2,colID2));
    results->restart_current();
    delete[] arr2->Array;
    delete arr1;
    delete arr2;
    return results;
}

list *JoinArray::Join(int relID1,int colID1,JoinArray *array2,int relID2,int colID2) {
    setrel(relID1);
    auto arr1 = sort(new radix(size,Array[relToBeJoined],rels->get_column(relID1,colID1)));
    array2->setrel(relID2);
    auto arr2 = sort(new radix(array2->size,array2->Array[array2->relToBeJoined],rels->get_column(relID2,colID2)));
    list *results=join(arr1,arr2,rels->get_column(relID1,colID1),rels->get_column(relID2,colID2));
    results->restart_current();
    delete[] arr1->Array;
    delete[] arr2->Array;
    delete arr1;
    delete arr2;
    return results;
}

list *JoinArray::sortedJoin(int relID1,int colID1,JoinArray *array2,int relID2,int colID2) {
    setrel(relID1);
    auto arr1 = new rows_array(size,Array[relToBeJoined]);
    array2->setrel(relID2);
    auto arr2 = sort(new radix(array2->size,array2->Array[array2->relToBeJoined],rels->get_column(relID2,colID2)));
    list *results=sortedjoin(arr1,arr2,rels->get_column(relID1,colID1),rels->get_column(relID2,colID2));
    results->restart_current();
    delete[] arr2->Array;
    delete arr1;
    delete arr2;
    return results;
}

void JoinArray::update_array(list *results,int id) {
    uint64_t new_size=results->get_size();
    auto **new_array=new uint64_t*[numRels+1];
    for(int j=0; j<numRels+1; j++) new_array[j]=new uint64_t[new_size];
    rowids *rows;
    int *new_arrayID=new int[numRels+1]; //will contain every element of relationIDs and id
    int n=-1;
    for(int j=0; j<numRels; j++){   //add elements in the right order
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
    if(n==-1){  //new element is the largest and it is not added to the array
        n=numRels;
        new_arrayID[n]=id;
    }
    for(uint64_t i=0; i<new_size; i++){
        rows=results->pop();            //pop every element
        for(uint64_t j=0; j<numRels; j++){
            int temp=relationIDs[j];
            new_array[temp][i]=Array[j][rows->rowid1]; //keep from the old array the rows that are extracted from the list
        }
        new_array[n][i]=rows->rowid2;   //in every row add the value of the new column (new relation added)
        delete rows;
    }
    for(uint64_t i=0; i<numRels; i++) delete[] Array[i];
    delete[] Array;
    Array=new_array;
    size=new_size;
    numRels+=1;
    delete[] relationIDs;
    relationIDs=new_arrayID;
    delete results;
}

//it is similar to the above function, the only difference is that the new results are from array2
void JoinArray::update_array(list *results, JoinArray *array2) {
    uint64_t new_size=results->get_size();
    auto **new_array=new uint64_t*[numRels+1];
    for(int j=0; j<numRels+1; j++) new_array[j]=new uint64_t[new_size];
    rowids *rows;
    int *new_arrayID=new int[numRels+1];
    int n=-1;
    for(int j=0; j<numRels; j++){
        if(n<0 && relationIDs[j]>array2->relationIDs[0]){
            new_arrayID[j]=array2->relationIDs[0]; //array2 can't have more than one relation
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
        delete rows;
    }
    for(uint64_t i=0; i<numRels; i++) delete[] Array[i];
    delete[] Array;
    Array=new_array;
    size=new_size;
    numRels+=1;
    delete[] relationIDs;
    relationIDs=new_arrayID;
    delete results;
}

//creates the array after the first join (if no filter is used before)
void JoinArray::create_array(list *results,int id1,int id2) {
    size = results->get_size();
    Array = new uint64_t *[2];
    Array[0] = new uint64_t[size];
    Array[1] = new uint64_t[size];
    relationIDs = new int[2];
    bool op=false;
    numRels=2;
    if(id1<id2) { //id's must be in order
        relationIDs[0] = id1;
        relationIDs[1] = id2;
    }
    else{
        relationIDs[0] = id2;
        relationIDs[1] = id1;
        op=true;
    }
    for (uint64_t i = 0; i < size; i++) {
        rowids* temp= results->pop(); //extract every element from the list to the array
        if(op){
            Array[0][i] = temp->rowid2;
            Array[1][i] = temp->rowid1;
        }
        else{
            Array[0][i] = temp->rowid1;
            Array[1][i] = temp->rowid2;
        }
    }
    delete results;
}
