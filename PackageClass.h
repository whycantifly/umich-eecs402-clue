//Name:       Elaine Pang, Taylor Tappe, Colin Zarzycki
//Date:       04/13/2012
//Assignment: EECS402 Project 5
//Purpose:    PackageClass class definition

#ifndef _PACKAGECLASS_H_
#define _PACKAGECLASS_H_

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

#include <map>
#include "PlayerClass.h"
#include "SuggestionClass.h"
#include "BoardLocationClass.h"

class PackageClass
{
   public:
   
   // Pass game state (gameParticipants) back and forth
   string wrapSetupPkg(std::map<SuspectEnum, PlayerClass> gameParticipants);
   std::map<SuspectEnum, PlayerClass> unwrapSetupPkg(string wrappedPackage);
   
   // pass Case File back and forth
   string wrapCaseFilePkg(SuggestionClass caseFile);
   SuggestionClass unwrapCaseFilePkg(string caseFilePkg);
   
   // Pass currentPlayer back and forth
   string wrapCurrentPlayerPackage(SuspectEnum suspEnum);
   SuspectEnum unwrapCurrentPlayerPackage(string inputString);
   
   // Pass suspectEnum back and forth
   string wrapSuspectEnum(SuspectEnum suspectNum);
   SuspectEnum unwrapSuspectEnum(string suspectNumStr);
   
   // Test functions
   string wrapPackage();
   int unwrapPackage(string wrappedString);
   
   private:

};

#endif
