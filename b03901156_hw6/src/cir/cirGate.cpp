/****************************************************************************
  FileName     [ cirGate.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define class CirAigGate member functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdarg.h>
#include <cassert>
#include "cirGate.h"
#include "cirMgr.h"
#include "util.h"


using namespace std;

extern CirMgr *cirMgr;

// TODO: Implement memeber functions for class(es) in cirGate.h

/**************************************/
/*   class CirGate member functions   */
/**************************************/
size_t CirGate::globalRef = 0;

void
CirGate::reportGate() const
{	
	ostringstream out;
	cout<<"=================================================="<<endl;
	out<<"= "<<getTypeStr()<<"("<<varId<<")";
	if(symbol!=""){
		out<<"\""<<symbol<<"\"";
	}
	out<<", line "<<NoOfLine;
	cout<<setw(49)<<left<<out.str()<<right<<"="<<endl;
	cout<<"=================================================="<<endl;
}

void
CirGate::reportFanin(int level) 
{
   if(level >= 0){
   		CirGate::setGlobalRef();
   		this->fanInTr(this,0,level,0);
   }
   
   
}

void
CirGate::reportFanout(int level) 
{
   if(level >= 0){
   		CirGate::setGlobalRef();
   		this->fanOutTr(this,0,level,0);
   }
   
}

bool CirGate::haveFloatingFanIn(){
	for(size_t i=0;i<fanInList.size();i++){
   		if(fanInList[i].gate()->getTypeStr()=="UNDEF"){
   			return true;
   		}
   }
	return false;
}

void CirGate::fanInTr(CirGate* Gate, unsigned int presentLevel, unsigned int maxLevel, bool inversion){
	if(presentLevel <= maxLevel){
		for(int i=0;i<presentLevel;i++){
			cout<<"  ";
		}
	
	if(inversion){
		cout<<"!";
	}
	cout<<Gate->getTypeStr()<<" "<<Gate->varId;
	if(presentLevel == maxLevel || Gate->fanInList.empty()){
		cout<<endl;
		return;
	}
	if(Gate->isGlobalRef()){
		cout<<" (*)"<<endl;
	}
	else{
		cout<<endl;
		for(int i=0;i<Gate->fanInList.size();i++){
			fanInTr(Gate->fanInList[i].gate(),presentLevel+1,maxLevel,Gate->fanInList[i].inversion());
		}
	}
		Gate->setToGlobalRef();
	}
	return;
}

void CirGate::fanOutTr(CirGate* Gate, unsigned int presentLevel, unsigned int maxLevel, bool inversion){
	if(presentLevel <= maxLevel){
		for(int i=0;i<presentLevel;i++){
			cout<<"  ";
		}
	
	if(inversion){cout<<"!";}
		cout<<Gate->getTypeStr()<<" "<<Gate->varId;
	if(presentLevel == maxLevel || Gate->fanOutList.empty()){
		cout<<endl;
		return;
	}
	if(Gate->isGlobalRef()){
		cout<<" (*)"<<endl;
	}
	else{
		cout<<endl;
		for(int i=0;i<Gate->fanOutList.size();i++){
			fanOutTr(Gate->fanOutList[i].gate(),presentLevel+1,maxLevel,Gate->fanOutList[i].inversion());
		}
	}
	
}
	return;
}

void CirGate::printGate() const{
	if(varId==0) {
		cout<<"CONST0";
		return;
	}
	cout<<setw(4)<<left<<getTypeStr()<<varId;
	for(int i=0;i<fanInList.size();i++) {
		cout<<" ";
		if(fanInList[i].gate()->getType()==UNDEF_GATE){
			cout<<"*";
		}
		if(fanInList[i].inversion()){
			cout<<"!";
		}
			cout<<fanInList[i].gate()->varId;
		
	}
	if(symbol!=""){
		cout<<"("<<symbol<<")";
	}
}