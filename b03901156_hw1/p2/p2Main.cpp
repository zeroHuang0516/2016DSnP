#include "p2Table.h"
#include <iostream>
#include <string>
#include <stdlib.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>

using namespace std;

int main()
{
   Table table;

   // TODO: read in the csv file
   string csvFile;
   cout << "Please enter the file name: ";
   cin >> csvFile;
   if (table.read(csvFile))
      cout << "File \"" << csvFile << "\" was read in successfully." << endl;
   else exit(-1); // csvFile does not exist.

   // TODO read and execute commands
   string command;
   int num;
   while(true){
       cin>>command;
       if(command=="SUM"){
           cin>>num;
                if(!(num>=0&&num<table.getColSize())){
                   cout<<"Error: Illegal column index ("<<num<<")!!"<<endl;
                }
                else{
                    table.sum(num);
                }
           
       }
       else if(command=="AVE"){
           cin>>num;
                if(!(num>=0&&num<table.getColSize())){
                   cout<<"Error: Illegal column index ("<<num<<")!!"<<endl;
                }
                else{
                    table.ave(num);
                }
          
       }
       else if(command=="MAX"){
           cin>>num;
                if(!(num>=0&&num<table.getColSize())){
                   cout<<"Error: Illegal column index ("<<num<<")!!"<<endl;
                }
                else{
                     table.max(num);
                }
       }
       else if(command=="MIN"){
           cin>>num;
                if(!(num>=0&&num<table.getColSize())){
                   cout<<"Error: Illegal column index ("<<num<<")!!"<<endl;
                }
                else{
                     table.min(num);
                }
       }
       else if(command=="COUNT"){
           cin>>num;
                if(!(num>=0&&num<table.getColSize())){
                   cout<<"Error: Illegal column index ("<<num<<")!!"<<endl;
                }
                else{
                    table.count(num);
                }
       }
       else if(command=="ADD"){
           table.add();
       }
       else if(command=="PRINT"){
           table.print();
       }
       else if(command=="EXIT"){
          exit(-1);
       }
       else{
           cout<<"Error: unknown command: \""<<command<<"\""<<endl;
       }
   }
  
   return 0;
}
