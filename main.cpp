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

JoinArray *join(SQL *sql,Relations *relations){
   /* //ap ti main tou part 1
    radix r1(0,array1->Size,array1->Array,_array1->Array,1);
    sort(&r1);
    radix r2(0,array2->Size,array2->Array,_array2->Array,1);
    sort(&r2);
    list *resultlist=join(array1,array2);
    resultlist->print();
    //resultlist->printSize();
    //cout<<countResults(array1,array2,count1,count2)<<"\n"; //without saving results to list
    delete array1;
    delete array2;
    delete _array1;
    delete _array2;
    delete resultlist;
   Predicate *predicate;*/
   while(){

   }
}

int main(int argc, char *argv[]) {

    /*char *filename;
    if(argc!=2) {  //get filenames
        cout<<"Wrong amount of arguments\n";
        exit(-1);
    }
	filename=argv[1];

    Relations *relations=new Relations(filename);*/
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
            //results->clear();
        }
        else{
            sql=new SQL(line);
           // results->add(join(sql,relations));
            delete sql;
        }

    }

    return 0;
}
