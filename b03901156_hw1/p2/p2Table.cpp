#include "p2Table.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <algorithm>

using namespace std;

Row::Row(int rowSize){
    size=rowSize;
    _data=new int [size];
}
Row::Row(const Row& rowToCopy){
    size=rowToCopy.size;
    _data=new int[size];
    for(size_t i=0;i<size;i++){
        _data[i]=rowToCopy._data[i];
    }
}
Row::~Row(){
    delete[] _data;
}



const int Row::operator[] (size_t i) const{
        return _data[i];
}
int& Row::operator[] (size_t i){
        return _data[i];
}


const Row& Table::operator[] (size_t i) const{
    return _rows[i];
}
Row& Table::operator[] (size_t i){
    return _rows[i];
}


int Table::getColSize(){
    return colSize;
}

bool Table::read(const string& csvFile)
{
    string line,field;
    ifstream in;
    in.open(csvFile.c_str());
    
    if(in.is_open()){
        getline(in,line,'\r');
        stringstream ss(line);
        while(getline(ss,field,',')){
                    colSize++;
            }
        line.c_str();
        if(line[line.size()-1]==','){
            colSize+=1;
        }
    }
    in.close();
    
    in.open(csvFile.c_str());
    if(in.is_open()){
        Row row(colSize);
        while(getline(in,line,'\r')){
          if(line.size()==0){break;}
          else{
            stringstream stream(line);
            for(size_t i=0;i<colSize;i++){
            
                getline(stream,field,',');
                    if(field.size()==0){
                        row[i]=-100;
                    }
                    else{
                        row[i]=atoi(field.c_str());
                    }     
            }
            _rows.push_back(row);
            rowSize++;
          }
        }
        //cout<<"ROW: "<<rowSize<<endl;
        //cout<<"COL: "<<colSize<<endl;
        return true; 
    }
    else{
        return false;
    }    
}
void Table::print(){
    for(size_t i=0;i<_rows.size();i++){
        for(size_t j=0;j<colSize;j++){
            if(_rows[i][j]!=-100){
                cout<<right<<setw(4)<<_rows[i][j];
            }
            else{
                cout<<right<<setw(4)<<"";
            }
        }
        cout<<endl;
    }
}

void Table::sum(int idx){
    int SUM=0;
    for(size_t i=0;i<rowSize;i++){
        if(_rows[i][idx]!=-100){
            SUM+=_rows[i][idx];
        }
    }
    cout<<"The summation of data in column #"<<idx<<" is "<<SUM<<"."<<endl;
}

void Table::max(int idx){
    int MAX=_rows[0][idx];
    for(size_t i=0;i<rowSize;i++){
        if(_rows[i][idx]>MAX){
            MAX=_rows[i][idx];
        }
    }
    cout<<"The maximum of data in column #"<<idx<<" is "<<MAX<<"."<<endl;
}

void Table::min(int idx){
    for(size_t i=0;i<rowSize;i++){
        if(_rows[i][idx]==-100){
            _rows[i][idx]=1000;
        }
    }
    int MIN=_rows[0][idx];
    for(size_t j=0;j<rowSize;j++){
        if(_rows[j][idx]<MIN){
            MIN=_rows[j][idx];
        }
    }
    for(size_t i=0;i<rowSize;i++){
        if(_rows[i][idx]==1000){
            _rows[i][idx]=-100;
        }
    }
    cout<<"The minimum of data in column #"<<idx<<" is "<<MIN<<"."<<endl;
}

void Table::count(int idx){
     int COUNT=0;
     vector<int>collector;
     for(size_t i=0;i<rowSize;i++){
         if(_rows[i][idx]!=-100 && find(collector.begin(),collector.end(),_rows[i][idx])==collector.end() ){
             collector.push_back(_rows[i][idx]);
         }
     }
     COUNT=collector.size();
     cout<<"The distinct count of data in column #"<<idx<<" is "<<COUNT<<"."<<endl;
}

void Table::ave(int idx){
    double AVE=0;
    double num=0;
    for(size_t i=0;i<rowSize;i++){
        if(_rows[i][idx]!=-100){
            AVE+=_rows[i][idx];
            num++;
        }
    }
    AVE/=num;
    cout<<"The average of data in column #"<<idx<<" is "<<fixed<<setprecision(1)<<AVE<<"."<<endl;
}

void Table::add(){
    string data;
    Row newRow(colSize);
    for(size_t i=0;i<colSize;i++){
        cin>>data;
        if(data=="-"){
            newRow[i]=-100;
        }
        else{
            newRow[i]=atoi(data.c_str());
        }
    }
    _rows.push_back(newRow);
    rowSize+=1;
}