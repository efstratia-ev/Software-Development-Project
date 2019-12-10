#include<iostream>
#include <cstring>
#include <string>
#include"SQL.h"


SQL::SQL(char *line) {
    query = new char[(strlen(line)+1)];
    strcpy(query,line);
    query[strlen(line)]='\0';
    where_predicates = new Priority_Queue();
    CutQueryToParts();
}


int SQL::CutQueryToParts(){
   string select,from,where,q=(query);
   size_t pos;

   //get from
   pos = q.find('|');
   from = q.substr (0,pos);
   cout<<from<<endl;
   InitFromArrays(from);
   GetFromArrays(from);

   //get where
   q = q.substr(pos+1);
   pos = q.find('|');
   where = q.substr (0,pos);
   cout<<where<<endl;
   SplitWherePredicates(where);
   filters=where_predicates->InitRearrange();

   //get select
   select = q.substr(pos+1);
   cout<<select<<endl;
   InitSelectResults(select);
   SplitSelectResults(select);
   return 1;
}

int SQL::InitFromArrays(string from){
   int sum=1;
   for (auto x : from){
      if(x == ' ' ) sum++;
   }
   from_arrays_sz =sum;
   from_arrays= new int[from_arrays_sz];
}

void SQL::GetFromArrays(string from){
   size_t pos = from.find( ' ' );
   size_t pos_start = 0;
   int i=0;

   // Decompose statement
   while( pos != std::string::npos ) {
      from_arrays[i++]=stoi(from.substr(pos_start, pos-pos_start ),nullptr,10);
      cout<<from_arrays[i-1]<<endl;
      pos_start = pos + 1;

      pos = from.find( ' ', pos_start );
   }
   from_arrays[i++]=stoi(from.substr( pos_start, std::min( pos, from.size() ) - pos_start + 1),nullptr,10);
   cout<<from_arrays[i-1]<<endl;

}

void SQL::SplitWherePredicates(string where){
    size_t pos = where.find( '&' );
    size_t pos_start = 0;
    string predicate;

    // Decompose statement
    while( pos != std::string::npos ) {
        predicate=where.substr(pos_start, pos-pos_start );
        isFilter(predicate)  ? GetWhereFilters(predicate) : GetWherePredicates(predicate) ;
        pos_start = pos + 1;
        pos = where.find( '&', pos_start );
    }
    predicate=where.substr( pos_start, std::min( pos, where.size() ) - pos_start + 1);
    isFilter(predicate)  ? GetWhereFilters(predicate) : GetWherePredicates(predicate) ;
}

bool SQL::isFilter(string predicate){
    int sum=0;
    for (auto x : predicate){
        if(x == '.' ) sum++;
    }
    return  sum == 2 ? false :  true;
}

void SQL::GetWherePredicates(string predicate) {
    int a1,c1,a2,c2;
    size_t pos_start = 0;
    size_t pos = predicate.find( '.' );
    a1=stoi(predicate.substr(pos_start, pos-pos_start ),nullptr,10);

    pos_start = pos + 1;
    pos = predicate.find( '=' , pos_start );
    c1=stoi(predicate.substr(pos_start, pos-pos_start ),nullptr,10);

    pos_start = pos + 1;
    pos = predicate.find( '.' , pos_start );
    a2=stoi(predicate.substr(pos_start, pos-pos_start ),nullptr,10);

    pos_start = pos + 1;
    c2=stoi(predicate.substr( pos_start,  predicate.size()  - pos_start + 1),nullptr,10);
    where_predicates->Push(new join(a1,c1,a2,c2));

}

void SQL::GetWhereFilters(string predicate){
    int a,c,number;
    char comp;
    size_t pos_start = 0;
    size_t pos = predicate.find( '.' );
    a=stoi(predicate.substr(pos_start, pos-pos_start ),nullptr,10);

    pos_start = pos++ + 1;
    while(predicate[pos]>='0' && predicate[pos]<='9') pos++; //skip until comparison
    c=stoi(predicate.substr(pos_start, pos-pos_start ),nullptr,10);

    comp=predicate[pos];

    pos_start = pos + 1;
    number=stoi(predicate.substr(pos_start, pos-pos_start ),nullptr,10);

    where_predicates->Push(new comparison(a,c,comp,number));

}

int SQL::InitSelectResults(string select){
    int sum=1;
    for (auto x : select){
        if(x == ' ' ) sum++;
    }
    select_result_sz = sum;
    select_results= new set[select_result_sz];
}

void SQL::SplitSelectResults(string select){
    size_t pos = select.find( ' ' );
    size_t pos_start = 0;
    int i=0;

    // Decompose statement
    while( pos != std::string::npos ) {
        GetSelectResults(select.substr(pos_start, pos-pos_start),i++);
        pos_start = pos + 1;

        pos = select.find( ' ', pos_start );
    }
    GetSelectResults(select.substr( pos_start, std::min( pos, select.size() ) - pos_start + 1),i);
}

void SQL::GetSelectResults(string select,int i){
    int a,c;
    size_t pos_start = 0;
    size_t pos = select.find( '.' );
    a=stoi(select.substr(pos_start, pos-pos_start ),nullptr,10);


    pos_start = pos + 1;
    c=stoi(select.substr( pos_start,  select.size()  - pos_start + 1),nullptr,10);

    select_results[i].setArray(a);
    select_results[i].setColumn(c);
}

Predicate *SQL::getPredicate() {
    if(where_predicates->IsEmpty()) return NULL;
    return where_predicates->Pop();
}
