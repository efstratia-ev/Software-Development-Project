#ifndef PROJECT2019_RESULTSARRAY_H
#define PROJECT2019_RESULTSARRAY_H


class ResultsArray {
    int size;
    int rowIDs;
    int **Array;
    int currentColumn;
public:
    ResultsArray(int s,int r){
        size=s;
        rowIDs=r;
        Array=new int*[size];
    }
    void set_currentColumn(int column){
        currentColumn=column;
    }
    int get_value(int i){
        return Array[i][currentColumn];
    }
    void insert_row(int i,int *row){
        Array[i]=row;
    }
};


#endif
