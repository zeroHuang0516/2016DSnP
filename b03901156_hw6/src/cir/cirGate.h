/****************************************************************************
  FileName     [ cirGate.h ]
  PackageName  [ cir ]
  Synopsis     [ Define basic gate data structures ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CIR_GATE_H
#define CIR_GATE_H

#include <string>
#include <vector>
#include <iostream>
#include "cirDef.h"

using namespace std;

class CirGate;


//------------------------------------------------------------------------
//   Define classes
//------------------------------------------------------------------------
// TODO: Define your own data members and member functions, or classes
class mycirGate{
public:
    #define NEG 0x1
    mycirGate(CirGate* g, size_t phase):
      _gateV((size_t)g + phase) { }
    CirGate* gate() const {
      return (CirGate*)(_gateV & ~size_t(NEG)); 
    }
    bool inversion() const { return (_gateV & NEG); }
private:
    size_t _gateV;
};





class CirGate
{
  friend CirMgr;
  
public:
   CirGate(unsigned int id, unsigned int l=0, bool visted = false): varId(id),NoOfLine(l),symbol(""),visted(visted),ref(globalRef){};
   virtual ~CirGate() {}

   //virtual functions
   virtual string getTypeStr() const =0;
   virtual unsigned int getGateId()const=0;
   virtual GateType getType()const=0;

   // Basic access methods
   unsigned getLineNo() const { return NoOfLine; }


   //deal with symbol function
  void setSymbol(string sym){symbol=sym; }
  string getSymbol() {return symbol; }
  bool haveSymbol(){return (symbol!=""); }

  //check floating fan tin funciton
    bool haveFloatingFanIn();
   
  void StateOfTr(bool traced) {visted = traced;}
  bool isTraced()const {return visted;}
  bool used()const{
      return (getType()==PO_GATE)||!(fanOutList.empty());
  }

   // Printing functions
   void reportGate() const;
   void reportFanin(int level) ;
   void reportFanout(int level) ;
   void printGate()const;

   bool isGlobalRef(){return ref == globalRef;}
   static void setGlobalRef(){ ++globalRef;}
   void setToGlobalRef(){ref = globalRef;}
   



private:

protected:
    unsigned int varId, NoOfLine;
    string symbol;
    bool visted;

    size_t ref;
    static size_t globalRef;
    
    vector<mycirGate> fanInList, fanOutList;

    void fanInTr(CirGate*, unsigned int, unsigned int, bool);
    void fanOutTr(CirGate*, unsigned int, unsigned int, bool);
};


class aigGATE:public CirGate{
public:
  aigGATE(unsigned int id,unsigned int i,unsigned int l=0):
        CirGate(id,l),IdOfGate(i){};
  ~aigGATE(){};
  virtual string getTypeStr()const{return "AIG";}
  virtual unsigned int getGateId()const{return IdOfGate;}
  virtual GateType getType()const{return AIG_GATE;}
private:
  unsigned int IdOfGate;
};

class piGATE:public CirGate{
public:
  piGATE(unsigned int id,unsigned int l=0):CirGate(id,l){};
  ~piGATE(){};
  virtual string getTypeStr()const{return "PI";}
  virtual unsigned int getGateId()const{return -1;}
  virtual GateType getType()const{return PI_GATE;}
};

class poGATE:public CirGate{
public:
  poGATE(unsigned int id,unsigned int l=0):CirGate(id,l){};
  ~poGATE(){};
  virtual string getTypeStr() const {return "PO";}
  virtual unsigned int getGateId()const{return -1;}
  virtual GateType getType()const{return PO_GATE;}
};

class constGATE:public CirGate{
public:
  constGATE():CirGate(0){visted = true;};
  ~constGATE(){};
  virtual string getTypeStr()const{return "CONST";}
  virtual unsigned int getGateId()const{return -1;}
  virtual GateType getType()const{return CONST_GATE;}
};

class undefGATE:public CirGate{
public:
  undefGATE(unsigned int id):CirGate(id){};
  ~undefGATE(){};
  virtual string getTypeStr()const{return "UNDEF";}
  virtual unsigned int getGateId()const{return -1;}
  virtual GateType getType()const{return UNDEF_GATE;}
};


#endif // CIR_GATE_H
