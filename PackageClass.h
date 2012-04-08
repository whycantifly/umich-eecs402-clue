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
   
   string wrapPackage();
   
   string wrapSetupPkg(std::map<SuspectEnum, PlayerClass> gameParticipants, SuggestionClass caseFile, SuspectEnum thisSuspect);
   
   int unwrapPackage(string wrappedString);
   
   private:

};

#endif _PACKAGECLASS_H