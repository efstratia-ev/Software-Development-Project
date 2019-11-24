#include<iostream>
#include"TranslateSQL.h"

using namespace std;

void getFROM(string line,int * &Arrays){
    int i=0,arraySize=1;

    size_t pos =line.find("|");
    string temp =line.substr(0,pos);
    while(temp[i++]!='\0'){
        if(temp[i-1]==' ') arraySize++;
    }
    Arrays=new int[arraySize];
    for(i=0;i<arraySize;i++) {
        Arrays[i] = findArrays(i,temp);
        cout << Arrays[i] << "||" << endl;
    }
    //delete[] Arrays;
}

int findArrays(int pos,string line){
    int number=0,spaces=0,i=0;
    while(spaces!=pos)  if (line[i++] == ' ') spaces++; //skip until pos
    number+=(line[i++] - '0');
    while(line[i]!=' ' && line[i]!='\0'){
        number*=10;
        number+=(line[i++] - '0');
    }
    return number;
}

void getWHERE(string line,int ** &Predicates,int * Arrays){
    int i=1,arraySize=1;

    size_t pos =line.find("|");
    string temp =line.substr(pos);
    while(temp[i]!='|'){
        if(temp[i]=='&') arraySize++;
        i++;
    }
    Predicates= new int*[arraySize];
    for(int j = 0; j < arraySize; j++) {
        Predicates[j] = new int[COLUMNSPRE];
        findPredicates(temp,Predicates,j);
    }
    makePredicates(arraySize,Predicates,Arrays);
    for(int j = 0; j < arraySize; j++) {
        cout<<Predicates[j][0]<<" "<<Predicates[j][1]<<" "<<Predicates[j][2]<<" "<<Predicates[j][3]<<" "<<endl;
    }
}

void makePredicates(int arraySize,int ** &Predicates,int * Arrays){
    int i,j;
    for(i=0;i<arraySize;i++){
        for(j=0; j<COLUMNSPRE;j+=2) {
            if( Predicates[i][j]>0) Predicates[i][j]=Arrays[Predicates[i][j]];
        }
    }
}


void findPredicates(string line,int ** &Predicates,int pos){
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


void getSELECT(string line,int ** &ArraysResults,int * Arrays){
    int i=0,arraySize=1;
    size_t pos =line.find("|");
    string temp =line.substr(pos+1);
    pos =temp.find("|");
    temp =temp.substr(pos);
    while(temp[i++]!='\0'){
        if(temp[i-1]==' ') arraySize++;
    }
    ArraysResults= new int*[arraySize];
    for(int j = 0; j < arraySize; j++) {
        ArraysResults[j] = new int[COLUMNSRE];
        findArraysResults(temp,ArraysResults,j);
    }
    makeArraysResults(arraySize,ArraysResults,Arrays);
    //delete[] Arrays;
    for(int j = 0; j < arraySize; j++) {
        cout<<ArraysResults[j][0]<<" "<<ArraysResults[j][1]<<endl;
    }
}


void findArraysResults(string line,int ** &ArraysResults,int pos){
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

void makeArraysResults(int arraySize,int ** &ArraysResults,int * Arrays){
    int i;
    for(i=0;i<arraySize;i++){
        ArraysResults[i][0]=Arrays[ArraysResults[i][0]];
    }
}