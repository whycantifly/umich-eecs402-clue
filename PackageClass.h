#ifndef _PACKAGECLASS_H
#define _PACKAGECLASS_H

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
   
   // Functions to implement in Clue
   //string wrapSetupPkg(std::map<SuspectEnum, PlayerClass> gameParticipants, SuggestionClass caseFile, SuspectEnum thisSuspect);
   string wrapSetupPkg(std::map<SuspectEnum, PlayerClass> gameParticipants);
   std::map<SuspectEnum, PlayerClass> unwrapSetupPkg(string wrappedPackage);
   
   // Test functions
   string wrapPackage();
   int unwrapPackage(string wrappedString);
   
   private:

};

#endif _PACKAGECLASS_H