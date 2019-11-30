#include<iostream>
#include"SQLInfo.h"

using namespace std;

SQLInfo::SQLInfo(char *l) {
    filters=0; //na metraei ta filtra
    current =0;
    int next;
    line=l;
    next=getFROM();
    next=getWHERE(next);
    getSELECT(next);
}

int SQLInfo::getFROM() {
    int i=0;
    arraySize=1;

    while(line[i++]!='|'){
        if(line[i-1]==' ') arraySize++;
    }
    int next=i;
    Arrays=new int[arraySize];
    for(i=0;i<arraySize;i++) {
        Arrays[i] = findArrays(i);
        cout << Arrays[i] << "||" << endl;
    }
    return next;
}

int SQLInfo::findArrays(int pos) {
    int number=0,spaces=0,i=0;
    while(spaces!=pos)  if (line[i++] == ' ') spaces++; //skip until pos
    number+=(line[i++] - '0');
    while(line[i]!=' ' && line[i]!='|'){
        number*=10;
        number+=(line[i++] - '0');
    }
    return number;
}

int SQLInfo::getWHERE(int start) {
    int i=start;
    Predicates_sz=1;

    while(line[i]!='|'){
        if(line[i]=='&') Predicates_sz++;
        i++;
    }
    int next=i+1;
    Predicates= new int*[Predicates_sz];
    for(int j = 0; j < Predicates_sz; j++) {
        Predicates[j] = new int[COLUMNSPRE];
        findPredicates(j);
    }
    makePredicates();
    for(int j = 0; j < Predicates_sz; j++) {
        cout<<Predicates[j][0]<<" "<<Predicates[j][1]<<" "<<Predicates[j][2]<<" "<<Predicates[j][3]<<" "<<endl;
    }
    return next;
}

void SQLInfo::makePredicates() {
    int i,j;
    for(i=0;i<Predicates_sz;i++){
        for(j=0; j<COLUMNSPRE;j+=2) {
            if( Predicates[i][j]>0) Predicates[i][j]=Arrays[Predicates[i][j]];
        }
    }
}

void SQLInfo::findPredicates(int pos) { //allagi?
    int number=0,ands=0,i=1,j=0,compare=-3;
    while(ands!=pos)  if (line[i++] == '&') ands++; //skip until pos
    number=(line[i++] - '0');
    while(j!=COLUMNSPRE) {
        if(line[i] == '<') compare=-1;
        else if( line[i] == '>') compare=-2;
        if (line[i] != '.' && line[i] != '=' && line[i] != '<' && line[i] != '>' && line[i] != '&' && line[i] != '|') {
            number *= 10;
            number += (line[i++] - '0');
            continue;
        }
        if(j==2 && line[i]!='.'){
            Predicates[pos][j++]=compare;
            Predicates[pos][j]=number;
            break;
        }
        Predicates[pos][j++]=number;
        i++;
        number=(line[i++] - '0');
    }
}

void SQLInfo::getSELECT(int start) {
    int i=start;
    Arrayresults_sz=1;
    while(line[i++]!='\0'){
        if(line[i-1]==' ') Arrayresults_sz++;
    }
    ArraysResults= new int*[Arrayresults_sz];
    for(int j = 0; j < Arrayresults_sz; j++) {
        ArraysResults[j] = new int[COLUMNSRE];
        findArraysResults(j);
    }
    makeArraysResults();
    for(int j = 0; j < Arrayresults_sz; j++) {
        cout<<ArraysResults[j][0]<<" "<<ArraysResults[j][1]<<endl;
    }
}

void SQLInfo::findArraysResults(int pos) {
    int number=0,ands=0,i=1,j=0;
    while(ands!=pos)  if (line[i++] == ' ') ands++; //skip until pos
    number=(line[i++] - '0');
    while(j!=COLUMNSRE) {
        if (line[i] != '.' && line[i] != ' ' && line[i] != '\0') {
            number *= 10;
            number += (line[i++] - '0');
            continue;
        }
        ArraysResults[pos][j++]=number;
        i++;
        number=(line[i++] - '0');
    }

}

void SQLInfo::makeArraysResults() {
    int i;
    for(i=0;i<Arrayresults_sz;i++){
        ArraysResults[i][0]=Arrays[ArraysResults[i][0]];
    }
}

SQLInfo::~SQLInfo() {
    delete[] Arrays;
    for(int i=0; i<Predicates_sz; i++) delete Predicates[i];
    delete Predicates;
    for(int i=0; i<Arrayresults_sz; i++) delete ArraysResults[i];
    delete ArraysResults;
}

int *SQLInfo::get_next_filter() {
    if(current==filters) return NULL;
    else return Predicates[current++];
}

int *SQLInfo::get_next_join() {
    if(current==Predicates_sz) return NULL;
    else return Predicates[current++];
}
