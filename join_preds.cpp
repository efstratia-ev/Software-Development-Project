#include<iostream>
#include<fstream>
#include <cstdlib>
#include <string>
#include <vector>
#include <cstring>
#include "join_preds.h"

//returns nulls if not found
JoinArray *searchFiltered(int relation,int max,JoinArray **filtered) {
    if (!filtered)
        return nullptr;
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
    if (pred == nullptr)
        return nullptr;
    int rel1=pred->get_array(),rel2=pred->get_array2();
    int col1 = pred->get_column(),col2 = pred->get_column2();
    if ((result = searchFiltered(rel1,max,filtered)) != nullptr)
        result->joinUpdate(rel1,col1,rel2,col2,searchFiltered(rel2,max,filtered));
    else if ((result = searchFiltered(rel2,max,filtered)) != nullptr) 
        result->joinUpdate(rel2,col2,rel1,col1,searchFiltered(rel1,max,filtered));
    else  {
        auto arr1 = sort(new radix(rels->get_relRows(rel1),rels->get_column(rel1,pred->get_column())));
        auto arr2 = sort(new radix(rels->get_relRows(rel2),rels->get_column(rel2,pred->get_column2())));
        list *resultlist=join(arr1,arr2,rels->get_column(rel1,pred->get_column()),rels->get_column(rel2,pred->get_column2()),0);
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