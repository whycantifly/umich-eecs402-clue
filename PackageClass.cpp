#include "PackageClass.h"

// PRETTY MUCH EVERYTHING IN THIS CLASS IS NON-FUNCTIONAL EXCEPT FOR TESTING

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

// Beta wrapping initial package to send to client human player
string PackageClass::wrapSetupPkg(std::map<SuspectEnum, PlayerClass> gameParticipants, SuggestionClass caseFile, SuspectEnum thisSuspect)
{
  // Iterator through the map
  
  PlayerClass player;
  BoardLocationClass boardloc;
  int x;
  int y;
  bool aiflag;
  
  map< SuspectEnum, PlayerClass >::iterator partIter;
  
  //print out map contents
  for (partIter = gameParticipants.begin(); partIter != gameParticipants.end(); partIter++) ;
  {
    cout << "Suspect: " << partIter->first << endl;
  }
    
  int mapsize = (int) gameParticipants.size();
  partIter = gameParticipants.begin();
  
  // Put inside loop?
  player = partIter->second;
  boardloc = player.getPlayerLocation();
  x = boardloc.getXCoord();
  y = boardloc.getYCoord();
  aiflag = player.getAiFlag();
  partIter++;
  
  cout << "This suspect is: " << thisSuspect << endl;
  return("tada");
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