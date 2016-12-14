/****************************************************************************
  FileName     [ cirMgr.h ]
  PackageName  [ cir ]
  Synopsis     [ Define circuit manager ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-2014 LaDs(III), GIEE, NTU, Taiwan ]
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

/**********************/
/*   struct and enum  */
/**********************/
struct gateEle
{ 
    unsigned int _LHS;
    unsigned int _RHS1;
    unsigned int _RHS2;
};

enum formatType { 
    ASCII,
    BINARY,

    DUMMYFORMAT
};

class CirMgr
{
public:
   CirMgr():_gateDeclareLineNo(2),
    _bDFSd(false),_bStrashed(false),_bSimd(false){};
   ~CirMgr()
   {
       for (size_t i = 0;i<_totalList.size();++i) {
           if(_totalList[i] != NULL)
               delete _totalList[i];
           _totalList[i] = NULL;
       }

       for(size_t i=0;i<_POs.size();++i)
       {
           delete _POs[i];
           _POs[i] = NULL;
       }
   }

   // Access functions
   // return '0' if "gid" corresponds to an undefined gate.
   CirGate* getGate(unsigned gid) const ;

   // Member functions about circuit construction
   bool readCircuit(const string&);
   
   // Member functions about circuit reporting
   void printSummary() const;
   void printNetlist(); 
   void printPIs() const;
   void printPOs() const;
   void printFloatGates() const;
   void writeAag(ostream&) ;

private:

    //header
    formatType _cirFormat;
    unsigned int _M;
    unsigned int _I;
    unsigned int _L;
    unsigned int _O;
    unsigned int _A;

    //
    GateList _totalList;
    GateList _dfsList;

    //input
    IdList _PIIds;//literal id

    //output
    vector<POGate*> _POs;
    IdList _POIds;//literal id

    //And Gate
    vector<gateEle> _andGateIds;//literal id

    //traversal
    //unsigned int _netListId;
    unsigned int _gateDeclareLineNo;

    //write
    IdList _tracedAndGateIds;

    //flags
    bool _bDFSd;
    bool _bStrashed;
    bool _bSimd;

    /******Helper function******/
    //Parse
    bool parseHeader(istream&);
    bool parseInput (istream&);
    bool parseOutput(istream&);
    bool parseAIGGate(istream&);
    bool parseSymbol(istream&);
    void parseCheck()const;
    
    bool genConnection();
    bool genDfsList();

    CirGate* findGate(unsigned int gid);
    void dfsTraversal(CirGate* curGate);
    void traversalReset();
};


#endif // CIR_MGR_H