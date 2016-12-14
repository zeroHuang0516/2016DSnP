/****************************************************************************
  FileName     [ cirMgr.h ]
  PackageName  [ cir ]
  Synopsis     [ Define circuit manager ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CIR_MGR_H
#define CIR_MGR_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "cirGate.h"

using namespace std;

#include "cirDef.h"

extern CirMgr *cirMgr;

// TODO: Define your own data members and member functions
struct gateInfo{
  unsigned int LHS, RHS1, RHS2;
};

class CirMgr
{
public:
   CirMgr():LineNumOfGate(2){};
   ~CirMgr() {
      for(int i=0;i<circuitList.size();i++){
          if(circuitList[i]!=NULL){
            delete circuitList[i];
              circuitList[i]=NULL;
          }
      }
      for(int i=0;i<POs.size();i++){
          delete POs[i];
          POs[i] = NULL;
      }
   }

   // Access functions
   // return '0' if "gid" corresponds to an undefined gate.
   CirGate* getGate(unsigned int)const;

   // Member functions about circuit construction
   bool readCircuit(const string&);

   // Member functions about circuit reporting
   void printSummary() const;
   void printNetlist() const;
   void printPIs() const;
   void printPOs() const;
   void printFloatGates() const;
   void writeAag(ostream&) const;

private:
  //header variables
  int M, I, L, O, A;

  GateList circuitList;
  GateList dfsList;

  IdList PI_ids, PO_ids;
  vector<poGATE*>POs;

  vector<gateInfo> andGate_ids;

  IdList TrAndGate_ids;
  unsigned int LineNumOfGate;


  CirGate* findGate(unsigned int gid);
  void dfs(CirGate* Gate);

};

#endif // CIR_MGR_H
