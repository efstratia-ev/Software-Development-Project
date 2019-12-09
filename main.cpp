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

int checkArray(JoinArray **arrays,int max,int id){
    for(int j=0; j<max; j++){
        if(arrays[j] && arrays[j]->exists(id)){
            return j;
        }
    }
    return -1;
}

JoinArray *join(SQL *sql,Relations *relations){
   Predicate *predicate;
   JoinArray *results=NULL,**filter_results;
   int filters=sql->get_filters_num(),max=0;
   filter_results=new JoinArray*[filters];
   bool exists;
   for(int i=0; i<filters; i++){
       int curr;
       predicate=sql->getPredicate();
       exists=false;
       if((curr=checkArray(filter_results,max,predicate->get_array()))>=0) exists=true;
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
   list *res;
   while((predicate=sql->getPredicate())){
       if(results) results->print();
       if(predicate->is_filter()){
           results->compare(predicate->get_array(),predicate->get_column(),predicate->get_array2(),predicate->get_column2());
           continue;
       }
       int array1=predicate->get_array(),array2=predicate->get_array2();
       if(!results){
           int curr=checkArray(filter_results,max,array1);
           if(curr!=-1){
               results=filter_results[curr];
               filter_results[curr]=NULL;
           }
           else{
               curr=checkArray(filter_results,max,array2);
               if(curr!=-1){
                   results=filter_results[curr];
                   filter_results[curr]=NULL;
               }
           }
       }
       if(results && results->exists(array1)){
           int curr=checkArray(filter_results,max,array2);
           if(curr==-1){
               res=results->Join(array1,predicate->get_column(),array2,predicate->get_column2());
               results->update_array(res,array2);
           }
           else{
               res=results->Join(array1,predicate->get_column(),filter_results[curr],array2,predicate->get_column2());
               results->update_array(res,filter_results[curr]);
               delete filter_results[curr];
               filter_results[curr]=NULL;
           }
           continue;
       }
       if(results && results->exists(array2)){
           int curr=checkArray(filter_results,max,array1);
           if(curr==-1){
               res=results->Join(array2,predicate->get_column2(),array1,predicate->get_column());
               results->update_array(res,array1);
           }
           else{
               res=results->Join(array2,predicate->get_column2(),filter_results[curr],array1,predicate->get_column());
               results->update_array(res,filter_results[curr]);
               delete filter_results[curr];
               filter_results[curr]=NULL;
           }
           continue;
       }
       auto arr1 = relations->relation(array1)->col(predicate->get_column());
       sort(new radix(relations->getSize(),arr1->Array));
       auto arr2 = relations->relation(array2)->col(predicate->get_column2());
       sort(new radix(relations->getSize(),arr2->Array));
       list *resultlist=join(arr1,arr2);
       results=new JoinArray(relations);

       delete arr1;
       delete arr2;
       delete resultlist;
   }
   return results;
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
       if (!line) continue;
       if (strcmp(line, "Done") == 0 ) break;
       else if (strcmp(line, "F") == 0) {
           //print sum for each relation in the list
           results->clear();
       }
       else{
           sql=new SQL(line);
           results->add(join(sql,relations));
           delete sql;
       }

   }

   return 0;
}
