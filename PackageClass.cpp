//Name:       Elaine Pang, Taylor Tappe, Colin Zarzycki
//Date:       04/13/2012
//Assignment: EECS402 Project 5
//Purpose:    PackageClass member function definitions
//Info:       Class for wrapping and unwrapping strings to be passed back
//            and forth for networking purposes

#include "PackageClass.h"

// Wraps game state into string
string PackageClass::wrapSetupPkg(std::map<SuspectEnum, PlayerClass>
    gameParticipants)
{ 
  PlayerClass player;
  int suspNum;
  int i;
  string playerToString;
  stringstream ss;
  map< SuspectEnum, PlayerClass >::iterator partIter;
    
  // Get number of people in game
  int numPlayers = (int) gameParticipants.size();
  // Open SS and start w/ num players
  ss << numPlayers << " " ;
  
  // Set iterator to first player (host)
  partIter = gameParticipants.begin();
  // Put inside loop?
  for (i = 0; i < numPlayers; i++)
  {
  suspNum = partIter->first;
  ss << suspNum << " ";
  player = partIter->second;
  playerToString =  player.printPlayerToString();
  ss << playerToString << " " << endl;
  partIter++;
  }

  return(ss.str());
}

// Wraps case file into string
string PackageClass::wrapCaseFilePkg(SuggestionClass caseFile)
{ 
  stringstream ss;
  
  // Put info into a stream
  ss << caseFile.getSuspect() << " " << caseFile.getWeapon() << " " <<
      caseFile.getRoom() << endl;

  return(ss.str());
}

// Unwrap case file
SuggestionClass PackageClass::unwrapCaseFilePkg(string caseFilePkg)
{ 
  istringstream iss;
  SuggestionClass caseFile;
  
  int suspect, weapon, room;
  
  iss.str(caseFilePkg);
  
  iss >> suspect;
  iss >> weapon;
  iss >> room;
  
  caseFile = SuggestionClass(SuspectEnum(suspect), WeaponEnum(weapon),
      RoomEnum(room));

  return(caseFile);
}

// Wrap suspectEnum
string PackageClass::wrapSuspectEnum(SuspectEnum suspectNum)
{ 
  stringstream ss;
  
  // Put info into a stream
  ss << suspectNum << endl;

  return(ss.str());
}

// Unwrap SuspectEnum
SuspectEnum PackageClass::unwrapSuspectEnum(string suspectNumStr)
{ 
  istringstream iss;
  
  int suspectNum;
  
  iss.str(suspectNumStr);
  
  iss >> suspectNum;

  return(SuspectEnum(suspectNum));
}

// Here we unwrap the gameParticipants string which was passed
std::map<SuspectEnum, PlayerClass> PackageClass::unwrapSetupPkg(string
    wrappedPackage)
{
  std::map<SuspectEnum, PlayerClass> gameParticipants;
  map< SuspectEnum, PlayerClass >::iterator partIter;
  
  string singlePlayerString;
  istringstream iss;
  
  int i; // Loop index
  int numPlayers;
  int suspNumber;
  
  gameParticipants.clear();
  
  // Set wrapped package to iss
  iss.str(wrappedPackage);
  
  // Get number of players
  iss >> numPlayers;
  
  for (i = 0; i < numPlayers; i++)
  {
    PlayerClass player;
    
    // Extract suspect number
    iss >> suspNumber;
    
    // Get singlePlayerString
    getline( iss, singlePlayerString );
    cout << singlePlayerString << endl;
    
    // Pass string to PlayerClass to roll up
    player.stringToPlayer(singlePlayerString);
    
    gameParticipants[SuspectEnum(suspNumber)] = player;
  }
  
  return(gameParticipants);
}

// Test to wrap a few things
string PackageClass::wrapCurrentPlayerPackage(SuspectEnum suspEnum)
{
  stringstream ss;
  ss << suspEnum << " " << endl;
  
  cout << ss.str() << endl;

  return (ss.str());
}

SuspectEnum PackageClass::unwrapCurrentPlayerPackage(string inputString)
{
  SuspectEnum suspEnum;
  istringstream iss;
  int suspect;
  
  iss.str(inputString);
  
  iss >> suspect;
  
  suspEnum = SuspectEnum(suspect);

  return(suspEnum);
}