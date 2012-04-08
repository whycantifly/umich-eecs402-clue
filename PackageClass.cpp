#include "PackageClass.h"

// PRETTY MUCH EVERYTHING IN THIS CLASS IS NON-FUNCTIONAL EXCEPT FOR TESTING

// Beta wrapping initial package to send to client human player
string PackageClass::wrapSetupPkg(std::map<SuspectEnum, PlayerClass> gameParticipants, SuggestionClass caseFile, SuspectEnum thisSuspect)
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
  ss << numPlayers << " " << endl;
  
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
  
  cout << ss.str() << endl;
  return(ss.str());
}

void PackageClass::unwrapSetupPkg(string package)
{
return;
}

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