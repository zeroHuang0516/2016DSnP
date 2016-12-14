/****************************************************************************
  FileName     [ memCmd.cpp ]
  PackageName  [ mem ]
  Synopsis     [ Define memory test commands ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2007-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/
#include <iostream>
#include <iomanip>
#include <sstream>
#include "memCmd.h"
#include "memTest.h"
#include "cmdParser.h"
#include "util.h"

using namespace std;

extern MemTest mtest;  // defined in memTest.cpp

bool
initMemCmd()
{
   if (!(cmdMgr->regCmd("MTReset", 3, new MTResetCmd) &&
         cmdMgr->regCmd("MTNew", 3, new MTNewCmd) &&
         cmdMgr->regCmd("MTDelete", 3, new MTDeleteCmd) &&
         cmdMgr->regCmd("MTPrint", 3, new MTPrintCmd)
      )) {
      cerr << "Registering \"mem\" commands fails... exiting" << endl;
      return false;
   }
   return true;
}


//----------------------------------------------------------------------
//    MTReset [(size_t blockSize)]
//----------------------------------------------------------------------
CmdExecStatus
MTResetCmd::exec(const string& option)
{
   // check option
   string token;
   if (!CmdExec::lexSingleOption(option, token))
      return CMD_EXEC_ERROR;
   if (token.size()) {
      int b;
      if (!myStr2Int(token, b) || b < int(toSizeT(sizeof(MemTestObj)))) {
         cerr << "Illegal block size (" << token << ")!!" << endl;
         return CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
      }
      #ifdef MEM_MGR_H
      mtest.reset(toSizeT(b));
      #else
      mtest.reset();
      #endif // MEM_MGR_H
   }
   else
      mtest.reset();
   return CMD_EXEC_DONE;
}

void
MTResetCmd::usage(ostream& os) const
{  
   os << "Usage: MTReset [(size_t blockSize)]" << endl;
}

void
MTResetCmd::help() const
{  
   cout << setw(15) << left << "MTReset: " 
        << "(memory test) reset memory manager" << endl;
}


//----------------------------------------------------------------------
//    MTNew <(size_t numObjects)> [-Array (size_t arraySize)]
//----------------------------------------------------------------------
CmdExecStatus
MTNewCmd::exec(const string& option)
{
   // TODO .. ok
  //check option
  size_t arraySize=0;
  int numObjects=0;
  size_t repeat=0;
  int tmp;
  vector<string> options;
  if (!CmdExec::lexOptions(option, options))
      return CMD_EXEC_ERROR;
  if (options.empty())
      return CmdExec::errorOption(CMD_OPT_MISSING, "");
  
  if(options.size()==1){
      if (!myStr2Int(options[0], numObjects)) {
        return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[0]);
      }
      if(numObjects == 0){
        cerr<<"Error: Illegal option!! (0)"<<endl;
      }
      else if(numObjects<0){
        cerr<<"Error: Illegal option!! ("<<numObjects<<")"<<endl;
      }
      else{
        mtest.newObjs(numObjects);
      }
  }
  else if(options.size()==2){
  	return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[0]);
  }
  else if(options.size()>3){
        return CmdExec::errorOption(CMD_OPT_EXTRA,options[3] );
  }
  else if(options.size()==3){
  		//-a 2 3
  		if (myStrNCmp("-Array", options[0], 2) == 0){
  			if (!myStr2Int(options[1], tmp)) {
              		cerr<<"Error: "<<"\""<<options[1]<<"\""<<" is not an integer!!"<<endl;
            		return CMD_EXEC_ERROR;
            	}
            	arraySize=tmp;
  			if (!myStr2Int(options[2], tmp)) {
              	cerr<<"Error: "<<"\""<<options[2]<<"\""<<" is not an integer!!"<<endl;
              	return CMD_EXEC_ERROR;
            }
            repeat=tmp;
  		}
  		//2 -a 3
  		else if (myStrNCmp("-Array", options[1], 2) == 0){
  			if (!myStr2Int(options[2], tmp)) {
              		cerr<<"Error: "<<"\""<<options[2]<<"\""<<" is not an integer!!"<<endl;
            		
            		return CMD_EXEC_ERROR;
            	}
            	arraySize=tmp;
  			if (!myStr2Int(options[0], tmp)) {
              	cerr<<"Error: "<<"\""<<options[0]<<"\""<<" is not an integer!!"<<endl;
              	
              	return CMD_EXEC_ERROR;
            }
            repeat = tmp;
  		}
  		else{
  			return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[0]);
  		}

      if(arraySize == 0 || repeat ==0 ){
        cerr<<"Error: Illegal option!! (0)"<<endl;
      }
      else if(arraySize<0){
        cerr<<"Error: Illegal option!! ("<<arraySize<<")"<<endl;
      }
      else if(repeat<0){
        cerr<<"Error: Illegal option!! ("<<repeat<<")"<<endl;
      }
      else{
          try{
              if(arraySize >=1){
                mtest.newArrs(repeat, arraySize);
              }
              
            }
          catch(std::bad_alloc e){
            return CMD_EXEC_ERROR;
          }
      }
      
  }
  
  return CMD_EXEC_DONE;
}

void
MTNewCmd::usage(ostream& os) const
{  
   os << "Usage: MTNew <(size_t numObjects)> [-Array (size_t arraySize)]\n";
}

void
MTNewCmd::help() const
{  
   cout << setw(15) << left << "MTNew: " 
        << "(memory test) new objects" << endl;
}


//----------------------------------------------------------------------
//    MTDelete <-Index (size_t objId) | -Random (size_t numRandId)> [-Array]
//----------------------------------------------------------------------
CmdExecStatus
MTDeleteCmd::exec(const string& option)
{
   // TODO ..ok
  vector<string> options;
  int numRandId=0 ;
  bool doArr = false;
  bool doRand = false;
  bool doIdx = false;
  int idx;
  int numR=0;
  int numI=0;
  int tmp;

  if (!CmdExec::lexOptions(option, options))
      return CMD_EXEC_ERROR;

  if (options.empty())
      return CmdExec::errorOption(CMD_OPT_MISSING, "");
  
  else {

  	for(int i=0;i<options.size();i++){
  		if(myStrNCmp("-Array", options[i],2 ) == 0){
        	doArr=true;
    	}
    	else if(myStrNCmp("-Index", options[i],2 ) == 0){
        	doIdx=true;
        	if(i+1<options.size()){
        		if (!myStr2Int(options[i+1], idx)) {
        			cerr<<"Error: "<<"\""<<options[i+1]<<"\""<<" is not an integer!!"<<endl;
        			return CMD_EXEC_ERROR;
      			}
      			if(idx<0){
        			cerr<<"Error: "<<"\""<<idx<<"\""<<" is not positive!!"<<endl;
        			return CMD_EXEC_ERROR;
      			}
        	}
        	numI++;
    	}
    	else if(myStrNCmp("-Random", options[i],2 ) == 0){
        	doRand=true;

        	if(i+1<options.size()){
        		if (!myStr2Int(options[i+1], numRandId)) {
        			
        			cerr<<"Error: "<<"\""<<options[i+1]<<"\""<<" is not an integer!!"<<endl;
        			return CMD_EXEC_ERROR;
      			}
      			if(numRandId<0){
        			cerr<<"Error: "<<"\""<<numRandId<<"\""<<" is not positive!!"<<endl;
        			return CMD_EXEC_ERROR;
      			}
        	}
        	numR++;
    	}
  	}
  }

  if(options.size()==1){
      return CmdExec::errorOption(CMD_OPT_MISSING, "");
  }

  else if(myStr2Int(options[0], tmp)){
  		return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[0]);
  }

  else if(numI>=2){
  		cerr<<"Error: Extra option!! (-i)"<<endl;
  		return CMD_EXEC_ERROR;
  }

  else if(numR>=2){
  		cerr<<"Error: Extra option!! (-r)"<<endl;
  		return CMD_EXEC_ERROR;
  }

  else if(numI==0 && numR==0){
  		return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[0]);
  }

  else if(doIdx && options.size()>2 && !doArr){
      return CmdExec::errorOption(CMD_OPT_EXTRA,options[2]);
  }

  else if(doRand && options.size()>2 && !doArr){
      return CmdExec::errorOption(CMD_OPT_EXTRA,options[2]);
  }

  else if(doIdx && options.size()>3 && doArr){
      return CmdExec::errorOption(CMD_OPT_EXTRA,options[3]);
  }

  else if(doRand && options.size()>3 && doArr){
      return CmdExec::errorOption(CMD_OPT_EXTRA,options[3]);
  }

  else if(!doArr && doIdx && idx>=mtest.getObjListSize()){
  		cerr << "Error: Object index " << idx << " is out of range!!\n";
  		return CMD_EXEC_ERROR;
  }

  else if(doArr && doIdx && idx>=mtest.getArrListSize()){
        cerr << "Error: Array index " << idx << " is out of range!!\n";
        return CMD_EXEC_ERROR;
  }

  else if(!doArr && doRand && mtest.getObjListSize() == 0){
  		cerr << "Error: Size of object list is 0!!\n"<<endl;
  		return CMD_EXEC_ERROR;
  }

  else if(doArr && doRand && mtest.getArrListSize() == 0){
        cerr << "Error: Size of array list is 0!!\n"<<endl;
        return CMD_EXEC_ERROR;
   }  

  else{
  		if(!doArr && doIdx && idx>=0 && numI==1 && numR==0 ){
  			mtest.deleteObj(idx);
  		}
  		else if(doArr && doIdx && idx>=0 && numI==1 && numR==0){
  			mtest.deleteArr(idx);
  		}
  		else if(doRand && !doArr && numRandId>0 && numR==1 && numI==0){
  			for(size_t i=0;i<numRandId;i++){
  				mtest.deleteObj(rnGen(mtest.getObjListSize()));
  			}
  		}
  		else if(doRand && doArr && numRandId>0 && numR==1 && numI==0){
  			for(size_t i=0;i<numRandId;i++){
  				mtest.deleteArr(rnGen(mtest.getArrListSize()));
  			}
  		}
  } 
  return CMD_EXEC_DONE;    
}

void
MTDeleteCmd::usage(ostream& os) const
{  
   os << "Usage: MTDelete <-Index (size_t objId) | "
      << "-Random (size_t numRandId)> [-Array]" << endl;
}

void
MTDeleteCmd::help() const
{  
   cout << setw(15) << left << "MTDelete: " 
        << "(memory test) delete objects" << endl;
}


//----------------------------------------------------------------------
//    MTPrint
//----------------------------------------------------------------------
CmdExecStatus
MTPrintCmd::exec(const string& option)
{
   // check option
   if (option.size())
      return CmdExec::errorOption(CMD_OPT_EXTRA, option);
   mtest.print();

   return CMD_EXEC_DONE;
}

void
MTPrintCmd::usage(ostream& os) const
{  
   os << "Usage: MTPrint" << endl;
}

void
MTPrintCmd::help() const
{  
   cout << setw(15) << left << "MTPrint: " 
        << "(memory test) print memory manager info" << endl;
}


