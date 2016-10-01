#ifndef P2_TABLE_H
#define P2_TABLE_H

#include <vector>
#include <string>


using namespace std;

class Row
{
public:
    Row(int rowSize);
    Row(const Row&);
    ~Row();
   const int operator[] (size_t i) const; // TODO
   int& operator[] (size_t i); // TODO

private:
   int  *_data;
   size_t size;
};

class Table
{
public:
   const Row& operator[] (size_t i) const;
   Row& operator[] (size_t i);
   int getColSize();
   void print();
   void sum(int idx);
   void ave(int idx);
   void max(int idx);
   void min(int idx);
   void count(int idx);
   void add();

   bool read(const string&);

private:
   vector<Row>  _rows;
   size_t colSize=0;
   size_t rowSize=0;
};





#endif // P2_TABLE_H

