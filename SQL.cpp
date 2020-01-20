#include <iostream>
#include <cstring>
#include <string>
#include <math.h>
#include <vector>
#include <limits.h>
#include"SQL.h"


SQL::SQL(char *line,Relations *rels) {
    this->rels = rels;
    query = new char[(strlen(line)+1)];
    strcpy(query,line);
    query[strlen(line)]='\0';
    where_predicates = new Priority_Queue();
    numInnerJoins = 0;
    QueryMinCost = LONG_MAX ;
    CutQueryToParts();
}


int SQL::CutQueryToParts(){
   string select,from,where,q=(query);
   size_t pos;

   //get from
   pos = q.find('|');
   from = q.substr (0,pos);
   InitFromArrays(from);
   GetFromArrays(from);
   rels->set_query_rels(from_arrays);

   //get where
   q = q.substr(pos+1);
   pos = q.find('|');
   where = q.substr (0,pos);
   SplitWherePredicates(where);
   filters=where_predicates->InitRearrange();
   //stats
   if(STATS) {
       auto indices = new int[numInnerJoins];
       for (int i = 0; i < numInnerJoins; i++) {
           indices[i] = i;
       }
       for (int i = 0; i < numInnerJoins; i++) {
           auto pr = where_predicates->getPredicateI(i);
           uint64_t garbage;
           pr->stats = applyJoinStats(rels, pr, garbage);
       }
       JoinsPermutations(indices, numInnerJoins, LONG_MAX);
       //print();
       where_predicates->RearrangeStats(joinOrder, numInnerJoins);
   }
   //get select
   select = q.substr(pos+1);
   InitSelectResults(select);
   SplitSelectResults(select);
   return 1;
}

void SQL::InitFromArrays(const string& from){
   int sum=1;
   for (auto x : from){
      if(x == ' ' ) sum++;
   }
   from_arrays_sz =sum;
   from_arrays= new int[from_arrays_sz];
}

void SQL::GetFromArrays(const string& from){
   size_t pos = from.find( ' ' );
   size_t pos_start = 0;
   int i=0;

   // Decompose statement
   while( pos != std::string::npos ) {
      from_arrays[i++]=stoi(from.substr(pos_start, pos-pos_start ),nullptr,10);
      pos_start = pos + 1;

      pos = from.find( ' ', pos_start );
   }
   from_arrays[i]=stoi(from.substr( pos_start, std::min( pos, from.size() ) - pos_start + 1),nullptr,10);

}

void SQL::SplitWherePredicates(const string& where){
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
    joinOrder = new int[numInnerJoins];
}

bool SQL::isFilter(const string& predicate){
    int sum=0;
    for (auto x : predicate){
        if(x == '.' ) sum++;
    }
    return sum != 2;
}

void updatRemainingColStats(Relation *rel,uint64_t prevTotalValues,int updatedCol) {
    auto sz = rel->getCols();
    auto st = rel->getColStats(updatedCol);
    stats *restStats;
    for (int i =0; i < sz; i++) {
        if (i == updatedCol)
            continue;
        restStats = rel->getColStats(i);
        if (st->getTotalValues() == 0) {
            restStats->setDistinctValues(0);
        } else {
            double totalVal = (double)st->getTotalValues();
            double prevTotalVal = (double)prevTotalValues;
            double restTotalVal = (double)restStats->getTotalValues();
            double restDistinctVal = (double)restStats->getDistinctValues();
            double temp = pow(1.0 - totalVal/prevTotalVal,restTotalVal/restDistinctVal);
            restStats->setDistinctValues(restDistinctVal * (1.0- temp));
            restStats->setTotalValues(st->getTotalValues());
        }
    }
}

void updatRemainingColStatsJoin(Relation *rel,int updatedCol,uint64_t prevTotalValues,uint64_t prevDisValues,uint64_t DisValues) {
    auto sz = rel->getCols();
    auto st = rel->getColStats(updatedCol);
    stats *restStats;
    for (int i =0; i < sz; i++) {
        if (i == updatedCol)
            continue;
        restStats = rel->getColStats(i);
        if (st->getTotalValues() == 0 ) {
            restStats->setDistinctValues(0);
        } else {
            if(prevDisValues==0 || restStats->getDistinctValues()==0)
                restStats->setDistinctValues(0);
            else{
                double temp = pow(1.0 - (double)DisValues/(double)prevDisValues ,(double)restStats->getTotalValues()/(double)restStats->getDistinctValues());
                restStats->setDistinctValues(restStats->getDistinctValues()*(1.0- temp));
            }
            restStats->setTotalValues(prevTotalValues);
        }
    }
}


void updatRemainingColStatsSelfJoin(Relation *rel,int updatedCol,uint64_t TotalValues) {
    auto sz = rel->getCols();
    auto st = rel->getColStats(updatedCol);
    stats *restStats;
    for (int i =0; i < sz; i++) {
        if (i == updatedCol)
            continue;
        restStats = rel->getColStats(i);
        if (st->getTotalValues() == 0) {
            restStats->setDistinctValues(0);
        } else {
            st->setTotalValues(TotalValues);
        }
    }
}

void swap(int &a, int &b){
    int temp=a;
    a=b;
    b=temp;
}

//evaluate new stats after join with predidcate `pr`
Relations *SQL::applyJoinStats(Relations *relsStats,Predicate * pr,uint64_t &cost ){
    Relations * results = new Relations(relsStats->getRels(),relsStats->getSize()); //TODO deletion TT
    results->set_query_rels(relsStats->getQueryRels());
    int a1=pr->getArray1()->getArray(),a2=pr->getArray2()->getArray();
    int c1=pr->getArray1()->getColumn(),c2=pr->getArray2()->getColumn();
    auto rel1 = relsStats->relation(a1),rel2 = relsStats->relation(a2);  //the arrays
    auto st1 = rel1->getColStats(c1),st2 = rel2->getColStats(c2); //the statistics
    auto prevDistValues1 = st1->getDistinctValues(), prevDistValues2 = st2->getDistinctValues();
    auto prevTotalValues1 = st1->getTotalValues(), prevTotalValues2 = st2->getTotalValues();
    auto tmpMin=st1->getMin() > st2->getMin() ? st1->getMin() : st2->getMin() ;
    auto tmpMax= st1->getMax() > st2->getMax() ? st2->getMax() : st1->getMax() ;
    results->relation(a1)->getColStats(c1)->setMax(tmpMax);
    results->relation(a1)->getColStats(c1)->setMin(tmpMin);
    results->relation(a2)->getColStats(c2)->setMax(tmpMax);
    results->relation(a2)->getColStats(c2)->setMin(tmpMin);
    auto n_tmp = st1->getMax()-st1->getMin()+1;
    if(a1==a2){
        if(c1==c2){
            results->relation(a1)->getColStats(c1)->setTotalValues(prevTotalValues1*prevTotalValues1/n_tmp);
            updatRemainingColStatsSelfJoin(results->relation(a1),c1,results->relation(a1)->getColStats(c1)->getTotalValues());
        }
        else{
            if(prevTotalValues1==0 || prevDistValues1==0)
                st1->setDistinctValues(0);
            else {
                auto temp = pow(1.0 - (double)st1->getTotalValues() / (double)prevTotalValues1, (double)prevTotalValues1 / (double)prevDistValues1);
                results->relation(a1)->getColStats(c1)->setDistinctValues(prevTotalValues1 * (1 - temp));
            }
            updatRemainingColStats(results->relation(a1),prevDistValues1,c1);

        }
    }
    else{
        auto x = prevTotalValues1*prevTotalValues2/n_tmp ;
        results->relation(a1)->getColStats(c1)->setTotalValues(x);
        results->relation(a2)->getColStats(c2)->setTotalValues(x);
        x=prevDistValues1*prevDistValues2/n_tmp;
        results->relation(a1)->getColStats(c1)->setDistinctValues(x);
        results->relation(a2)->getColStats(c2)->setDistinctValues(x);
        updatRemainingColStatsJoin(results->relation(a1),c1,prevTotalValues1,prevDistValues1,results->relation(a1)->getColStats(c1)->getDistinctValues());
        updatRemainingColStatsJoin(results->relation(a2),c2,prevTotalValues1,prevDistValues2,results->relation(a2)->getColStats(c2)->getDistinctValues());
    }
    cost=results->relation(a1)->getColStats(c1)->getTotalValues();
    return results;
}


void SQL::JoinsPermutations(int *indices,int size,uint64_t min) {
    if (size == 1 && CheckConnection(indices)) { //
        Relations *res;
        uint64_t cost=0;
        res = where_predicates->getPredicateI(indices[0])->stats;  //get the first stats

        for (int i =1; i < numInnerJoins; i++) {
            res = applyJoinStats(res,where_predicates->getPredicateI(indices[i]),cost);  //apply all the stats until the end
        }
        if (cost < QueryMinCost) {
            QueryMinCost = cost;
            for (int i = 0; i < numInnerJoins; i++)
                joinOrder[i] = indices[i];

        }
    }
    for (int i=0; i < size; i++) {
        JoinsPermutations(indices,size-1,min);
        if(size%2==1){
            swap(indices[0],indices[size-1]
            );
        }
        else {
            swap(indices[i], indices[size - 1]);
        }
    }
}


void SQL::GetWherePredicates(const string& predicate) {
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
    numInnerJoins++;
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

    if(STATS){
        auto rel = rels->relation(a);
        auto st = rel->getColStats(c);
        auto bm = st->getBitMap();
        auto prevTotalValues = st->getTotalValues();
        double temp;
        switch (comp) {
            case '=':
                st->setMax(number);
                st->setMin(number);
                if (bm->get(number)) {
                    st->setTotalValues(st->getTotalValues()/st->getDistinctValues());
                    st->setDistinctValues(1);
                } else  {
                    st->setDistinctValues(0);
                    st->setTotalValues(0);
                }
                break;
            case '>':
                if (number < st->getMin())
                    number = st->getMin();
                temp = (double)(st->getMax()-number)/(st->getMax()-st->getMin());
                st->setDistinctValues(st->getDistinctValues()*temp);
                st->setTotalValues(st->getTotalValues()*temp);
                st->setMin(number);
                break;
            case '<':
                if (number > st->getMax())
                    number = st->getMax();
                temp = (double)(number-st->getMin())/(st->getMax()-st->getMin());
                st->setDistinctValues(st->getDistinctValues()*temp);
                st->setTotalValues(st->getTotalValues()*temp);
                st->setMax(number);
                break;
            default:
                throw exception();
        }
        updatRemainingColStats(rel,prevTotalValues,c);

    }
    where_predicates->Push(new comparison(a,c,comp,number));
}

void SQL::InitSelectResults(const string& select){
    int sum=1;
    for (auto x : select){
        if(x == ' ' ) sum++;
    }
    select_result_sz = sum;
    select_results= new set[select_result_sz];
}

void SQL::SplitSelectResults(const string& select){
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

void SQL::GetSelectResults(const string& select,int i){
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
    if(where_predicates->IsEmpty()) return nullptr;
    return where_predicates->Pop();
}

int SQL::get_filters_num() {
    return filters;
}

int SQL::get_results_counter() {
    return select_result_sz;
}

set *SQL::get_select() {
    return select_results;
}

int *SQL::get_from_arrays() {
    return from_arrays;
}

SQL::~SQL() {
    delete[] joinOrder;
    delete[] query;
    delete[] from_arrays;
    delete where_predicates;
    delete[] select_results;
}

bool SQL::CheckConnection(int *indices){
    bool * existance = new bool[numInnerJoins];
    for (int i=0; i < numInnerJoins; i++) {
        existance[i]=false;
    }
    Predicate * current=where_predicates->getPredicateI(indices[0]);
    existance[current->getArray1()->getArray()]=true;
    existance[current->getArray2()->getArray()]=true;
    for (int i=1; i < numInnerJoins; i++) {
        current=where_predicates->getPredicateI(indices[i]);
        if(existance[current->getArray1()->getArray()]==false && existance[current->getArray2()->getArray()]==false){
            delete[] existance;
            return false;
        }
        existance[current->getArray1()->getArray()]=true;
        existance[current->getArray2()->getArray()]=true;
    }
    delete[] existance;
    return true;
}