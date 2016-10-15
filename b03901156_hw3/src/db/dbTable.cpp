/****************************************************************************
  FileName     [ dbTable.cpp ]
  PackageName  [ db ]
  Synopsis     [ Define database Table member functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2015-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iomanip>
#include <string>
#include <cctype>
#include <cassert>
#include <set>
#include <algorithm>
#include <climits>
#include <cctype>
#include <iostream>
#include "dbTable.h"

using namespace std;

/*****************************************/
/*          Global Functions             */
/*****************************************/
ostream& operator << (ostream& os, const DBRow& r)
{
   // Ok
   // - Data are seperated by a space. No trailing space at the end.
   // - Null cells are printed as '.'
   for(size_t i=0;i<r.size()-1;i++){
       if(r[i]==INT_MAX){os<<"."<<" ";}
       else{os<<r[i]<<" ";}
   }
   if(r[r.size()-1]==INT_MAX){os<<".";}
   else{os<<r[r.size()-1];}
   return os;
}

ostream& operator << (ostream& os, const DBTable& t)
{
   // Ok
   // - Data are seperated by setw(6) and aligned right.
   // - Null cells are printed as '.'
   for(size_t i=0;i<t.nRows();i++){
       for(size_t j=0;j<t.nCols();j++){
           if(t.getData(i,j)!=INT_MAX){
               os<<right<<setw(6)<<t.getData(i,j);
           }
           else{
               os<<right<<setw(6)<<".";
           }
       }
       cout<<endl;
   }
   return os;
}

ifstream& operator >> (ifstream& ifs, DBTable& t)
{
   //OK
   // TODO: to read in data from csv file and store them in a table
   // - You can assume all the data of the table are in a single line.
   int colSize=0;
   string line,field,_line;
   DBRow row;
       //to know colSize
         getline(ifs,line,'\r');
         stringstream ss(line);
        while(getline(ss,field,',')){
                      colSize++;
        }
        _line=line;
        _line.c_str();
        if(line[line.size()-1]==','){
            colSize+=1;
        }
         //read first line
           stringstream _ss(line);
          for(int i=0;i<colSize;i++){
                    getline(_ss,field,',');
                    if(field.size()>=1){
                        row.addData(atoi(field.c_str()));
                    }
                    else{
                        row.addData(INT_MAX);
                    }      
            }
            (t._table).push_back(row);
            row.reset();
        //read other lines
        while(getline(ifs,line,'\r')){
          if(line.size()==0){break;}
          else{
            stringstream stream(line);
            for(int i=0;i<colSize;i++){
                    getline(stream,field,',');
                    if(field.size()>=1){
                        row.addData(atoi(field.c_str()));
                    }
                    else{
                        row.addData(INT_MAX);
                    }     
            }
            (t._table).push_back(row);
            row.reset();
          }
        }
   return ifs;
}

/*****************************************/
/*   Member Functions for class DBRow    */
/*****************************************/
void  DBRow::removeCell(size_t c)
{
   // ok
   _data.erase(_data.begin()+c);
}

/*****************************************/
/*   Member Functions for struct DBSort  */
/*****************************************/
bool  DBSort::operator() (const DBRow& r1, const DBRow& r2) const
{
   // TODO: called as a functional object that compares the data in r1 and r2
   //       based on the order defined in _sortOrder .. OK
   for(size_t i=0;i<_sortOrder.size();i++){
      if(r1[_sortOrder[i]]<r2[_sortOrder[i]]){
          return(r1[_sortOrder[i]]<r2[_sortOrder[i]]);
          break;
      }
      else if(r1[_sortOrder[i]]>r2[_sortOrder[i]]){
          return false;
          break;
      }
     
   }
   return false;
}

/*****************************************/
/*   Member Functions for class DBTable  */
/*****************************************/
void  DBTable::reset()
{
   // ok
  vector<DBRow> tmp; 
  tmp.swap(_table);
}

void  DBTable::addCol(const vector<int>& d)
{
   // TODO: add a column to the right of the table. Data are in 'd'.
   //...ok
   for(size_t i=0;i<_table.size();i++){
       _table[i].addData(d[i]);
   }
}

void  DBTable::delRow(int c)
{
   // TODO: delete row #c. Note #0 is the first row...Ok
   _table.erase(_table.begin()+c);
}

void  DBTable::delCol(int c)
{
   // delete col #c. Note #0 is the first row.
   for (size_t i = 0, n = _table.size(); i < n; ++i)
      _table[i].removeCell(c);
}

// For the following getXXX() functions...  (except for getCount())
// - Ignore null cells
// - If all the cells in column #c are null, return NAN
// - Return "float" because NAN is a float.
float  DBTable::getMax(size_t c) const
{
   // TODO: get the max data in column #c..Ok
   bool empty=true;
   float MAX=_table[0][c];
    for(size_t i=0;i<_table.size();i++){
        if(_table[i][c]!=INT_MAX){empty=false;}
        if(_table[i][c]>MAX && _table[i][c]!=INT_MAX){
            MAX=_table[i][c];
        }
    }
    if(empty){
        MAX=NAN;
    }
   return MAX;
}

float
DBTable::getMin(size_t c) const
{
   // TODO: get the min data in column #c..Ok
     bool empty=true;
    float MIN=_table[0][c];
    for(size_t j=0;j<_table.size();j++){
        if(_table[j][c]!=INT_MAX){
            empty=false;
        }
        if(_table[j][c]<MIN){
            MIN=_table[j][c];
        }
    }
    if(empty){
        MIN=NAN;
    }
    
   return MIN;
}

float DBTable::getSum(size_t c) const
{
   // TODO: compute the sum of data in column #c..OK
   float SUM=0;
   bool empty=true;
    for(size_t i=0;i<_table.size();i++){
        if(_table[i][c]!=INT_MAX){
            SUM+=_table[i][c];
            empty=false;
        }
    }
    if(empty){
        SUM = NAN;
    }
   return SUM;
}

int  DBTable::getCount(size_t c) const
{
   // TODO: compute the number of distinct data in column #c..Ok
   // - Ignore null cells
     int COUNT=0;
     vector<int>collector;
     for(size_t i=0;i<_table.size();i++){
         if(_table[i][c]!=INT_MAX && find(collector.begin(),collector.end(),_table[i][c])==collector.end() ){
             collector.push_back(_table[i][c]);
         }
     }
     COUNT=collector.size();
   return COUNT;
}

float  DBTable::getAve(size_t c) const
{
   // TODO: compute the average of data in column #c..Ok
   float AVE=0;
   float num=0;
   bool empty=true;
    for(size_t i=0;i<_table.size();i++){
        if(_table[i][c]!=INT_MAX){
            empty=false;
        }
        if(_table[i][c]!=INT_MAX){
            AVE+=_table[i][c];
            num++;
        }
    }
    AVE/=num;
    if(empty){
        AVE=NAN;
    }
   return AVE;
}

void  DBTable::sort(const struct DBSort& s)
{
   // TODO: sort the data according to the order of columns in 's'..Ok
   std::sort(_table.begin(), _table.end(),s);
}

void  DBTable::printCol(size_t c) const
{
   // TODO: to print out a column...Ok
   // - Data are seperated by a space. No trailing space at the end.
   // - Null cells are printed as '.'
   for(size_t i=0;i<_table.size()-1;i++){
       if(_table[i][c]==INT_MAX){
           cout<<"."<<" ";
       }
       else{
           cout<<_table[i][c]<<" ";
       }
   }
   if(_table[_table.size()-1][c]==INT_MAX){
       cout<<".";
   }
   else{
       cout<<_table[_table.size()-1][c]<<endl;
   }
}




void  DBTable::printSummary() const{
   size_t nr = nRows(), nc = nCols(), nv = 0;
   for (size_t i = 0; i < nr; ++i)
      for (size_t j = 0; j < nc; ++j)
         if (_table[i][j] != INT_MAX) ++nv;
   cout << "(#rows, #cols, #data) = (" << nr << ", " << nc << ", "
        << nv << ")" << endl;
}

