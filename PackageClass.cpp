#include "PackageClass.h"

// PRETTY MUCH EVERYTHING IN THIS CLASS IS NON-FUNCTIONAL EXCEPT FOR TESTING

// Beta wrapping initial package to send to client human player
//string PackageClass::wrapSetupPkg(std::map<SuspectEnum, PlayerClass> gameParticipants, SuggestionClass caseFile, SuspectEnum thisSuspect)
string PackageClass::wrapSetupPkg(std::map<SuspectEnum, PlayerClass> gameParticipants)
{ 
  PlayerClass player;
  int suspNum;
  int i;
  string playerToString;
  stringstream ss;
  map< SuspectEnum, PlayerClass >::iterator partIter;
  
// This iterator not working for some reason...  
//  for (partIter = gameParticipants.begin(); partIter != gameParticipants.end(); partIter++) ;
//  {
//   cout << "Suspect: " << partIter->first << endl;
//  }
    
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

string PackageClass::wrapCaseFilePkg(SuggestionClass caseFile)
{ 
  stringstream ss;
  
  // Put info into a stream
  ss << caseFile.getSuspect() << " " << caseFile.getWeapon() << " " << caseFile.getRoom() << endl;

  return(ss.str());
}

SuggestionClass PackageClass::unwrapCaseFilePkg(string caseFilePkg)
{ 
  istringstream iss;
  SuggestionClass caseFile;
  
  int suspect, weapon, room;
  
  iss.str(caseFilePkg);
  
  iss >> suspect;
  iss >> weapon;
  iss >> room;
  
  caseFile = SuggestionClass(SuspectEnum(suspect), WeaponEnum(weapon), RoomEnum(room));

  return(caseFile);
}

string PackageClass::wrapSuspectEnum(SuspectEnum suspectNum)
{ 
  stringstream ss;
  
  // Put info into a stream
  ss << suspectNum << endl;

  return(ss.str());
}

SuspectEnum PackageClass::unwrapSuspectEnum(string suspectNumStr)
{ 
  istringstream iss;
  
  int suspectNum;
  
  iss.str(suspectNumStr);
  
  iss >> suspectNum;

  return(SuspectEnum(suspectNum));
}

// Here we unwrap the gameParticipants string which was passed
std::map<SuspectEnum, PlayerClass> PackageClass::unwrapSetupPkg(string wrappedPackage)
{
  std::map<SuspectEnum, PlayerClass> gameParticipants;
  map< SuspectEnum, PlayerClass >::iterator partIter;
  
  string singlePlayerString;
  istringstream iss;
  
  int i; // Loop index
  int numPlayers;
  int suspNumber;
  
  gameParticipants.clear();
  
//  cout << "The Wrapped Package is..." << endl;
//  cout << wrappedPackage << endl;
  
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

// map< SuspectEnum, PlayerClass >::iterator PackageClass::unwrapCurrentPlayerPackage(string inputString, std::map<SuspectEnum, PlayerClass> gameParticipants)
// {
//   map< SuspectEnum, PlayerClass >::iterator currentPlayerIter, outputPlayerIter;
//   SuspectEnum suspEnum;
//   istringstream iss;
//   int suspect;
//   int i;
//   int NUM_PLAYERS = 3;
//   
//   iss.str(inputString);
//   
//   iss >> suspect;
//   
//   suspEnum = SuspectEnum(suspect);
//   
//   currentPlayerIter = gameParticipants.begin();
//   
//   for (i = 0; i < NUM_PLAYERS; i++)
//   {
//     if (currentPlayerIter->first == suspEnum)
//     {
//       outputPlayerIter = currentPlayerIter;
//     }
//     currentPlayerIter++;
//   }
//   
//   return(outputPlayerIter);
// }



/////// TEST FUNCTIONS

// Test to wrap a few things
string PackageClass::wrapPackage()
{
  string str,strTotal;
  ifstream in;
  in.open("myfile.txt");
  getline(in,str);
  while(in)
  {
    strTotal += str;
    getline(in,str);
  }
  
  int myInt = 6;
  float myFloat = 12.12;
  string myString = "test_test_test";
  stringstream ss;
  ss << myInt << " " << myFloat << " " << myString << endl;
//  cout << ss.str();

  return (ss.str());
}

// Test unwrap
int PackageClass::unwrapPackage(string wrappedString)
{
  int myInt;
  float myFloat;
  string myString;
  istringstream iss;
  
  cout << "The Wrapped String is..." << endl;
  
  cout << wrappedString << endl;
  
  iss.str(wrappedString);
  iss >> myInt;
  iss >> myFloat;
  iss >> myString;
  
  cout << "Int val: " << myInt << endl;
  cout << "Float val: " << myFloat << endl;
  cout << "String val: " << myString << endl;
  
  return(myInt);
}