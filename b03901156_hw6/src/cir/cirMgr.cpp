/****************************************************************************
  FileName     [ cirMgr.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define cir manager functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iostream>
#include <iomanip>
#include <cstdio>
#include <ctype.h>
#include <cassert>
#include <cstring>
#include "cirMgr.h"
#include "cirGate.h"
#include "util.h"

using namespace std;

// TODO: Implement memeber functions for class CirMgr

/*******************************/
/*   Global variable and enum  */
/*******************************/
CirMgr* cirMgr = 0;

enum CirParseError {
   EXTRA_SPACE,
   MISSING_SPACE,
   ILLEGAL_WSPACE,
   ILLEGAL_NUM,
   ILLEGAL_IDENTIFIER,
   ILLEGAL_SYMBOL_TYPE,
   ILLEGAL_SYMBOL_NAME,
   MISSING_NUM,
   MISSING_IDENTIFIER,
   MISSING_NEWLINE,
   MISSING_DEF,
   CANNOT_INVERTED,
   MAX_LIT_ID,
   REDEF_GATE,
   REDEF_SYMBOLIC_NAME,
   REDEF_CONST,
   NUM_TOO_SMALL,
   NUM_TOO_BIG,

   DUMMY_END
};

/**************************************/
/*   Static varaibles and functions   */
/**************************************/
static unsigned lineNo = 0;  // in printint, lineNo needs to ++
static unsigned colNo  = 0;  // in printing, colNo needs to ++
static char buf[1024];
static string errMsg;
static int errInt;
static CirGate *errGate;

static bool
parseError(CirParseError err)
{
   switch (err) {
      case EXTRA_SPACE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Extra space character is detected!!" << endl;
         break;
      case MISSING_SPACE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Missing space character!!" << endl;
         break;
      case ILLEGAL_WSPACE: // for non-space white space character
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Illegal white space char(" << errInt
              << ") is detected!!" << endl;
         break;
      case ILLEGAL_NUM:
         cerr << "[ERROR] Line " << lineNo+1 << ": Illegal "
              << errMsg << "!!" << endl;
         break;
      case ILLEGAL_IDENTIFIER:
         cerr << "[ERROR] Line " << lineNo+1 << ": Illegal identifier \""
              << errMsg << "\"!!" << endl;
         break;
      case ILLEGAL_SYMBOL_TYPE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Illegal symbol type (" << errMsg << ")!!" << endl;
         break;
      case ILLEGAL_SYMBOL_NAME:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Symbolic name contains un-printable char(" << errInt
              << ")!!" << endl;
         break;
      case MISSING_NUM:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Missing " << errMsg << "!!" << endl;
         break;
      case MISSING_IDENTIFIER:
         cerr << "[ERROR] Line " << lineNo+1 << ": Missing \""
              << errMsg << "\"!!" << endl;
         break;
      case MISSING_NEWLINE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": A new line is expected here!!" << endl;
         break;
      case MISSING_DEF:
         cerr << "[ERROR] Line " << lineNo+1 << ": Missing " << errMsg
              << " definition!!" << endl;
         break;
      case CANNOT_INVERTED:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": " << errMsg << " " << errInt << "(" << errInt/2
              << ") cannot be inverted!!" << endl;
         break;
      case MAX_LIT_ID:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Literal \"" << errInt << "\" exceeds maximum valid ID!!"
              << endl;
         break;
      case REDEF_GATE:
         cerr << "[ERROR] Line " << lineNo+1 << ": Literal \"" << errInt
              << "\" is redefined, previously defined as "
              << errGate->getTypeStr() << " in line " << errGate->getLineNo()
              << "!!" << endl;
         break;
      case REDEF_SYMBOLIC_NAME:
         cerr << "[ERROR] Line " << lineNo+1 << ": Symbolic name for \""
              << errMsg << errInt << "\" is redefined!!" << endl;
         break;
      case REDEF_CONST:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Cannot redefine constant (" << errInt << ")!!" << endl;
         break;
      case NUM_TOO_SMALL:
         cerr << "[ERROR] Line " << lineNo+1 << ": " << errMsg
              << " is too small (" << errInt << ")!!" << endl;
         break;
      case NUM_TOO_BIG:
         cerr << "[ERROR] Line " << lineNo+1 << ": " << errMsg
              << " is too big (" << errInt << ")!!" << endl;
         break;
      default: break;
   }
   return false;
}

/**************************************************************/
/*   class CirMgr member functions for circuit construction   */
/**************************************************************/
CirGate* CirMgr::getGate(unsigned gid) const {
    
    if(gid<=M){
        if(circuitList[gid]!=NULL){
            return circuitList[gid];
        }
        return NULL;
    }
    else if(gid>M+O){
        return NULL;
    }
    else{
        if(POs[gid-M-1] == NULL){
            cout << "ERROR in getGate" << endl;
            return NULL;
        }
        return POs[gid-M-1];
    }
}

bool CirMgr::readCircuit(const string& fileName)
{
   ifstream infile;
   infile.open(fileName.c_str(),ios::in);
   unsigned int inputId;
   unsigned int outputId;
   gateInfo andGateId;

   if(infile.is_open()){
      char dummy[3];
      infile>>dummy[0]>>dummy[1]>>dummy[2]>>M>>I>>L>>O>>A;
      circuitList.resize(M+1,NULL);

      circuitList[0] = new constGATE;

      PI_ids.resize(I);
      for(int i=0;i<I;i++){
          unsigned int varId;
          infile>>inputId;
          varId = inputId/2;
          if(varId==0 || varId>M){ return false;}
              PI_ids[i]=inputId;
              circuitList[varId] = new piGATE(varId,LineNumOfGate);
              LineNumOfGate++;
          
      }

      PO_ids.resize(O);
      POs.resize(O);
      for(int i=0;i<O;i++){
          unsigned int varId=M+i+1;
          infile>>outputId;
          PO_ids[i] = outputId;
          POs[i]=new poGATE(varId,LineNumOfGate);
          LineNumOfGate++;
      }

      andGate_ids.resize(A);
      for(int i=0;i<A;i++){
          infile>>andGateId.LHS>>andGateId.RHS1>>andGateId.RHS2;
          unsigned int varId = andGateId.LHS/2;
          if(varId==0 || varId>M ){return false;}
              circuitList[varId] = new aigGATE(varId,i,LineNumOfGate);
              andGate_ids[i]=andGateId;
              LineNumOfGate++;
          
      }

      char check;
      infile>>check;
      if(check=='c'){
          string comment;
      }
      else if(check=='i'||check=='o'||check=='l'){
          char type=check;
          char dummy;
          unsigned int pos;
          string sb;
          infile>>dummy>>pos>>dummy>>sb;
          if(type == 'i'){
            unsigned int varId = PI_ids[pos]/2;
            circuitList[varId]->setSymbol(sb);
          }
          else if(type == 'o'){
            POs[pos]->setSymbol(sb);
          }
          else if(type == 'l'){}
          
          infile>>check;
          if(check=='c'){
              
          }
          else{
             
          }
      }
      else{
        
      }
      
    //record circuit
    for(int i=0;i<O;i++){
      POs[i]->fanInList.push_back(mycirGate(findGate(PO_ids[i]/2),PO_ids[i]%2));
      findGate(PO_ids[i]/2)->fanOutList.push_back(mycirGate(POs[i],PO_ids[i]%2));
    }
  for(int i=0;i<A;i++){
      CirGate* fanin1 = findGate(andGate_ids[i].RHS1/2);
      CirGate* fanin2 = findGate(andGate_ids[i].RHS2/2);
      circuitList[andGate_ids[i].LHS/2]->fanInList.push_back(mycirGate(fanin1,andGate_ids[i].RHS1%2));
      fanin1->fanOutList.push_back(mycirGate(circuitList[andGate_ids[i].LHS/2],andGate_ids[i].RHS1%2));
      circuitList[andGate_ids[i].LHS/2]->fanInList.push_back(mycirGate(fanin2, andGate_ids[i].RHS2%2));
      fanin2->fanOutList.push_back(mycirGate(circuitList[andGate_ids[i].LHS/2],andGate_ids[i].RHS2%2));
  } 
  
    CirGate::setGlobalRef();
    dfsList.clear();
    TrAndGate_ids.clear();
    
    dfsList.clear();
    TrAndGate_ids.clear();
    CirGate::setGlobalRef();
    if(circuitList.empty()&&POs.empty()){
        return false;
    }
    
    for(int i=0;i<POs.size();i++){
        dfs(POs[i]);
    }
    
      return true;
   }
   else
    {
        cerr << "Cannot open design \""  << fileName<< " \"!!" << endl; 
        return false;
    }
    
}

/**********************************************************/
/*   class CirMgr member functions for circuit printing   */
/**********************************************************/
/*********************
Circuit Statistics
==================
  PI          20
  PO          12
  AIG        130
------------------
  Total      162
*********************/
void
CirMgr::printSummary() const
{
    cout<<endl;
    cout<<"Circuit Statistics"<<endl;
    cout<<"=================="<<endl;
    cout<<"  PI    "<<setw(8)<<right<<I<<endl;
    cout<<"  PO    "<<setw(8)<<right<<O<<endl;
    cout<<"  AIG   "<<setw(8)<<right<<A<<endl;
    cout<<"------------------"<<endl;
    cout<<"  Total "<<setw(8)<<right<<I+O+A<<endl;
}

void
CirMgr::printNetlist() const
{
  cout<<endl;
  for(int i=0;i<dfsList.size();i++){
    cout<<"["<<i<<"]";
    // cout<<dfsList[i]->varId<<endl;
    dfsList[i]->printGate();
    cout<<endl;
  }
}

void
CirMgr::printPIs() const
{
   cout << "PIs of the circuit:";
   for(int i=0;i<I;i++){
    cout<<" "<<PI_ids[i]/2;
   }
   cout << endl;
}

void
CirMgr::printPOs() const
{
   cout << "POs of the circuit:";
   for(int i=0;i<O;i++){
    cout<<" "<<POs[i]->varId;
   }
   cout << endl;
}

void
CirMgr::printFloatGates() const
{ 
    bool first = true;
    bool unusedFirst = true;
    for(int i=1;i<circuitList.size();i++){
        if(circuitList[i]!=NULL){
            if(circuitList[i]->haveFloatingFanIn()){
                if(first){
                  cout<<"Gates with floating fanin(s):";
                  first=false;
                }
                cout<<" "<<i;
            }
        }
    }
    for(int i=0;i<POs.size();i++){
        
            if(POs[i]->haveFloatingFanIn()){
                if(first){
                  cout<<"Gates with floating fanin(s):";
                  first=false;
                }
                cout<<" "<<POs[i]->varId;
            }
        
    }
    for(int i=1;i<circuitList.size();i++){
      if(circuitList[i]!=NULL){
        if(circuitList[i]->used()==false){
            if(unusedFirst){
                cout<<"\nGates defined but not used :";
                unusedFirst = false;
            }
            cout<<" "<<i;
        }
      }
    }
}

void
CirMgr::writeAag(ostream& outfile) const
{
    outfile << M << " " << I << " " << L << " " << O << " " 
            << TrAndGate_ids.size()<< endl;
    for(size_t i=0;i<I;++i)
    {
        outfile << PI_ids[i] << endl;
    }
    for(size_t i=0;i<O;++i)
    {
        outfile << PO_ids[i] << endl;
    }
    for(int i=0;i<TrAndGate_ids.size();i++){
        outfile<<andGate_ids[TrAndGate_ids[i]].LHS<<" "<<andGate_ids[TrAndGate_ids[i]].RHS1<<" "<<andGate_ids[TrAndGate_ids[i]].RHS2<<endl;
    }
    for(int i=0;i<PI_ids.size();i++){
      if(circuitList[PI_ids[i]/2]->haveSymbol()){
        outfile<<"i"<<i<<" "<<circuitList[PI_ids[i]/2]->getSymbol()<<endl;
      }
    }
    for(int i=0;i<POs.size();i++){
      if(POs[i]->haveSymbol()){
        outfile<<"o"<<i<<" "<<POs[i]->getSymbol()<<endl;
      }
    }
    outfile<<"c\n"<<"~Circuit created!!!!!!!!"<<endl;
} 



CirGate* CirMgr::findGate(unsigned int gid){
  if(gid<=M+O){
    CirGate* G = getGate(gid);
    if(G!=NULL){
        return G;
    }
    else{
        circuitList[gid]=new undefGATE(gid);
        return circuitList[gid];
    }
  }
}

void CirMgr::dfs(CirGate* G){
    GateType G_type = G->getType();
    if(G->getType()!=UNDEF_GATE && G->isGlobalRef()==false){
        for(int i=0;i<G->fanInList.size();i++){
            if(G->fanInList[i].gate()->getType()!=UNDEF_GATE && G->fanInList[i].gate()->isGlobalRef()==false){
              dfs(G->fanInList[i].gate());
            }
        }
        dfsList.push_back(G);
        G->StateOfTr(true);

        if(G_type == AIG_GATE){
            TrAndGate_ids.push_back(G->getGateId());
        }
        G->setToGlobalRef();
    }
}





