#include<iostream>
#include<fstream>
#include <cstdlib>
#include <string>
#include <vector>
#include <cstring>
#include "radix.h"
#include "stack.h"
#include "sort.h"
#include "array.h"
#include "relations.h"
#include "results_list.h"
#include "JoinArray.h"
#include "SQL.h"

// sed -i 's/,/ /g' filename

using namespace std;

//check array if they have been filtered
int isRelationFiltered(JoinArray **filtered,int max,int relation){
    for(int j=0; j<max; j++){
        if(filtered[j] && filtered[j]->exists(relation)){
            return j;
        }
    }
    return -1;
}

//returns nulls if not found
JoinArray *searchFiltered(int relation,int max,JoinArray **filtered) {
    for(int i=0; i<max; i++) {
        if(filtered[i] && filtered[i]->exists(relation)){
            return filtered[i];
        }
    }
    return nullptr;
}

JoinArray *joinFirstPredicate(JoinArray **filtered,SQL *sql,Relations *rels,int max) {
    JoinArray *result;
    auto pred = sql->getPredicate();
    int rel1=pred->get_array(),rel2=pred->get_array2();
    int col1 = pred->get_column(),col2 = pred->get_column2();
    if ((result = searchFiltered(rel1,max,filtered)) != nullptr)
        result->joinUpdate(rel1,col1,rel2,col2,searchFiltered(rel2,max,filtered));
    else if ((result = searchFiltered(rel2,max,filtered)) != nullptr) 
        result->joinUpdate(rel2,col2,rel1,col1,searchFiltered(rel1,max,filtered));
    else  {
       auto arr1 = rels->get_column(rel1,pred->get_column());
       sort(new radix(arr1->Size,arr1->Array));
       auto arr2 = rels->get_column(rel2,pred->get_column2());
       sort(new radix(arr2->Size,arr2->Array));
       list *resultlist=join(arr1,arr2);
       result=new JoinArray(rels);
       result->create_array(resultlist,rel1,rel2);
       delete arr1;
       delete arr2;
       delete resultlist;
    }
    return result;
}


JoinArray *joinPredicates(JoinArray **filtered,SQL *sql,Relations *rels,int max) {
    JoinArray *results = nullptr;
    Predicate *predicate;
    list *res;
    results = joinFirstPredicate(filtered,sql,rels,max);
    while((predicate=sql->getPredicate())){
        if(predicate->is_filter()){
            results->compare(predicate->get_array(),predicate->get_column(),predicate->get_array2(),predicate->get_column2());
        }
        int rel1=predicate->get_array(),rel2=predicate->get_array2();
        int col1 = predicate->get_column(),col2 = predicate->get_column2();
        if (results->exists(rel1)) {
            results->joinUpdate(rel1,col1,rel2,col2,searchFiltered(rel2,max,filtered));
        }
        if (results->exists(rel2)) {
            results->joinUpdate(rel2,col2,rel1,col1,searchFiltered(rel1,max,filtered));
        }
    }
    return results;
}

uint64_t *join(SQL *sql,Relations *relations){
   Predicate *predicate;
   JoinArray *results=NULL,**filter_results;
   int filters=sql->get_filters_num(),max=0;
   filter_results=new JoinArray*[filters];
   bool exists;
   for(int i=0; i<filters; i++){
       int curr;
       predicate=sql->getPredicate();
       exists=false;
       if((curr=isRelationFiltered(filter_results,max,predicate->get_array()))>=0) exists=true;
       if(!exists){
           curr=max;
           filter_results[curr]=new JoinArray(relations);
           max++;
       }
       if(predicate->is_comparison()){
            int comparison=predicate->get_comp();
            if(comparison=='='){
                if(exists) filter_results[curr]->equal(predicate->get_column(),predicate->get_value());
                else filter_results[curr]->create_array(relations->equal(predicate->get_array(),predicate->get_column(),predicate->get_value()),predicate->get_array());
            }
            else if(comparison=='>'){
                if(exists) filter_results[curr]->grater_than(predicate->get_column(),predicate->get_value());
                else filter_results[curr]->create_array(relations->grater_than(predicate->get_array(),predicate->get_column(),predicate->get_value()),predicate->get_array());
            }
            else{
                if(exists) filter_results[curr]->less_than(predicate->get_column(),predicate->get_value());
                else filter_results[curr]->create_array(relations->less_than(predicate->get_array(),predicate->get_column(),predicate->get_value()),predicate->get_array());
            }
            continue;
       }
       if(exists) filter_results[curr]->compare(predicate->get_array(),predicate->get_column(),predicate->get_column2());
       else filter_results[curr]->create_array(relations->filter(predicate->get_array(),predicate->get_column(),predicate->get_column2()),predicate->get_array());
   }
   results = joinPredicates(filter_results,sql,relations,max);
   int res_counter=sql->get_results_counter();
   uint64_t *sums=new uint64_t[res_counter];
   set *select=sql->get_select();
   for(int i=0; i<res_counter; i++){
       sums[i]=results->get_sum(select[i].getArray(),select[i].getColumn());
   }
   return sums;
}

int main(int argc, char *argv[]) {

   char *filename;
   if(argc!=2) {  //get filenames
       cout<<"Wrong amount of arguments\n";
       exit(-1);
   }
   filename=argv[1];

   Relations *relations=new Relations(filename);
   SQL *sql;
   results_list *results=new results_list();


   char *line= NULL;
   size_t size=0;
   while(true){
       cout << "Enter Input:" << endl;
       getline(&line, &size, stdin);
       line=strtok(line,"\n");
       if (!line) continue;
       if (strcmp(line, "Done") == 0 ) break;
       else if (strcmp(line, "F") == 0) {
           results->print();
           results->clear();
       }
       else{
           sql=new SQL(line);
           int count;
           results->add(sql->get_results_counter(),join(sql,relations));
           delete sql;
       }

   }

   return 0;
}
