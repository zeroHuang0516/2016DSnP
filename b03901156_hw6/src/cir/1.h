/****************************************************************************
  FileName     [ cirGate.h ]
  PackageName  [ cir ]
  Synopsis     [ Define basic gate data structures ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-2014 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CIR_GATE_H
#define CIR_GATE_H

#include <string>
#include <vector>
#include <iostream>
#include "cirDef.h"
#include <climits>
#include<sstream>
#include<iomanip>

using namespace std;

class CirGate;
//#define ERRID UINT_MAX
//------------------------------------------------------------------------
//   Define classes
//------------------------------------------------------------------------
// TODO: Define your own data members and member functions, or classes

class CirGateV
{
public:
    #define NEG 0x1
    CirGateV(const CirGate* gate,size_t phase):
        _gateV((size_t)gate + phase){}
    CirGate* gate() const {return (CirGate*)(_gateV & ~size_t(NEG));}
    bool isInv() const {return (_gateV & NEG);}
    //bool operator < (CirGateV& rhs)
    //{return this->gate()->_varId < rhs.gate()->_varId;}

private:
    size_t _gateV;
};
class CirGate
{ 
    friend CirMgr;
    
public:
   CirGate(unsigned int id,unsigned int l=0,bool bTraced = false):
            _varId(id),_lineNo(l),_colNo(0),_symbol(""),
            _bTraced(bTraced),_ref(_globalRef){};
   
   virtual ~CirGate() {};

   // Basic access methods
   virtual string getTypeStr() const=0;
   unsigned getLineNo() const { return _lineNo; }
   virtual unsigned int getIdIndex()const = 0;
   virtual GateType getType()const = 0; 

   void setFecGrpNum(size_t num) {_fecGrpNum = num;}
   size_t getFecGrpNum()const {return _fecGrpNum;}
   
   //property related
   void setTracedOrNot(bool traced) {_bTraced = traced;}
   bool isTraced()const {return _bTraced;}
   bool isUsed()const {
       return (getType()==PO_GATE) || !(_fanoutList.empty());}
   bool hasFloatingFanin()const;
   bool hasSymbol()const{return _symbol!="";}
   string getSymbol() const {return _symbol;}
   //symbol
   void setSymbol(string symbol) {_symbol = symbol;}
   string getSymbol() {return _symbol;}
   
   // Print functions
   void printGate() const;
   void reportGate() const;
   void reportFanin(int level);
   void reportFanout(int level);

   //traversal methods
   bool isGlobalRef(){return _ref == _globalRef;}
   static void setGlobalRef(){++_globalRef;}
   void setToGlobalRef(){_ref = _globalRef;}

   //other
   //void sortFan();
protected:
    unsigned int _varId;
    unsigned int _lineNo;
    unsigned int _colNo;
    string _symbol;
    
    //for sweep
    bool _bTraced;

    //for simulate
    size_t _fecGrpNum;
    
    //for traversal
    mutable size_t _ref;
    static size_t _globalRef;

    vector<CirGateV> _faninList;
    vector<CirGateV> _fanoutList;

    /*****Helper Function*****/
    void fanoutTraversal(CirGate*,unsigned int, unsigned int,bool);
    void faninTraversal(CirGate*,unsigned int, unsigned int,bool);

    
};

class AIGGate:public CirGate
{
public:
    AIGGate(unsigned int id,unsigned int i,unsigned int l=0):
        CirGate(id,l),_gateIdIndex(i){};
    ~AIGGate(){};
   virtual string getTypeStr()const {return "AIG";};
   virtual GateType getType()const{ return AIG_GATE;}
   virtual unsigned int getIdIndex()const{return _gateIdIndex;} 
private:
   unsigned int _gateIdIndex;
};

class PIGate:public CirGate
{
public:
    PIGate(unsigned int id,unsigned int l=0):CirGate(id,l){};
    ~PIGate(){};

   virtual string getTypeStr()const {return "PI";};
   virtual GateType getType()const{ return PI_GATE;}
   virtual unsigned int getIdIndex()const{return -1;}//error

};

class POGate:public CirGate
{
public:
    POGate(unsigned int id,unsigned int l=0):CirGate(id,l){};
    ~POGate(){};

   virtual string getTypeStr()const {return "PO";};
   virtual GateType getType()const{ return PO_GATE;}
   virtual unsigned int getIdIndex()const{return -1;}//error

};

class ConstGate:public CirGate
{
public:
    ConstGate():CirGate(0){_bTraced = true;};
    ~ConstGate(){};
    
   virtual string getTypeStr() const{return "CONST";};
   virtual GateType getType()const{ return CONST_GATE;}
   virtual unsigned int getIdIndex()const{return -1;}//error
};

class UndefGate:public CirGate
{
public:
    UndefGate(unsigned int id):CirGate(id){};
    ~UndefGate(){};

   virtual string getTypeStr() const{return "UNDEF";};
   virtual GateType getType()const{ return UNDEF_GATE;}
   virtual unsigned int getIdIndex()const{return -1;}//error

};
#endif // CIR_GATE_H